# include "Parser.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Parser::Parser() : _linesRead(0), _confFilename(), _serversContexts() {}

Parser::Parser (std::string const& _conf_filename) :  _linesRead(0), _confFilename(_conf_filename), _serversContexts() {

	parse();
}

Parser::Parser(Parser const &rhs) {
	*this = rhs;
}

Parser &	Parser::operator=(Parser const & rhs) {

	if (this != &rhs) {

		this->_linesRead = rhs.getLinesRead();
		this->_confFilename = rhs.getConfFileName();
		this->_serversContexts = rhs.getServerContexts();
	}
	return *this;
}

Parser::~Parser(void) {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

std::vector<ServerContext> const&	Parser::getServerContexts(void) const { return this->_serversContexts; }

std::string const&					Parser::getConfFileName(void) const { return this->_confFilename; }

size_t								Parser::getLinesRead(void) const { return this->_linesRead; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void	Parser::parse() {

	std::ifstream		file(_confFilename.c_str());
	std::stringstream	stream;

	file.open(_confFilename.c_str());
	if (!file.is_open())
		throw Parser::Error("Cannot open file " + _confFilename);
	
	stream << file.rdbuf();

	std::string line;
	while (std::getline(stream, line)) {

		this->_linesRead++;
		this->trimAndReplaceWhitespaces(line);

		#ifdef DEBUG_PARSER
			std::cout << "[main scope] line " << _linesRead << ": " << line << std::endl;
		#endif

		if (isCommentOrEmptyLine(line)) {
			continue;
		}
		if (isEndOfScope(line)) {
			break;
		}
		else if (line.find("server {") != std::string::npos) {
			ServerContext	newServerCtxt;
			this->parseServerContext(stream, newServerCtxt);
			this->_serversContexts.push_back(newServerCtxt);
		}
		else {
			this->buildAndThrowParamError(line);
		}
	}
}


typename Parser::DirectiveMap<ServerContext>::type Parser::initializeServerDirectives() {
    typename Parser::DirectiveMap<ServerContext>::type directiveMap;

	directiveMap["autoindex"] = &Parser::parseAutoindex<ServerContext>;
	directiveMap["cgi"] = &Parser::parseCgi<ServerContext>;
	directiveMap["client_max_body_size"] = &Parser::parseMaxBodySize<ServerContext>;
	directiveMap["root"] = &Parser::parseRoot<ServerContext>;
    directiveMap["listen"] = &Parser::parseListen<ServerContext>;
	directiveMap["error_page"] = &Parser::parseErrorPage<ServerContext>;
	directiveMap["index"] = &Parser::parseIndex<ServerContext>;
	directiveMap["authorized_methods"] = &Parser::parseAuthorizedMethods<ServerContext>;
    directiveMap["server_name"] = &Parser::parseServerName<ServerContext>;

    return directiveMap;
}

typename Parser::DirectiveMap<LocationContext>::type Parser::initializeLocationDirectives() {
    typename Parser::DirectiveMap<LocationContext>::type directiveMap;

	directiveMap["cgi"] = &Parser::parseCgi<LocationContext>;
	directiveMap["client_max_body_size"] = &Parser::parseMaxBodySize<LocationContext>;
	directiveMap["alias"] = &Parser::parseAlias<LocationContext>;
	directiveMap["root"] = &Parser::parseRoot<LocationContext>;
	directiveMap["error_page"] = &Parser::parseErrorPage<LocationContext>;
	directiveMap["index"] = &Parser::parseIndex<LocationContext>;
	directiveMap["authorized_methods"] = &Parser::parseAuthorizedMethods<LocationContext>;

    return directiveMap;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: UTILS::

void Parser::isValidIPv4(const std::string& ip) const {
	
	std::istringstream iss(ip);
	std::string octet;
	std::string err("Invalid IP address " + ip);
	int count = 0;

	while (std::getline(iss, octet, '.')) {
		++count;

		if (count > 4) {
			throw Parser::InvalidParam(err, *this);
		}

		for (std::string::const_iterator it = octet.begin(); it != octet.end(); ++it) {
			if (!std::isdigit(*it)) {
				throw Parser::InvalidParam(err, *this);
			}
		}

		int num = std::atoi(octet.c_str());
		if (num < 0 || num > 255) {
			throw Parser::InvalidParam(err, *this);
		}
	}

	if (count != 4) {
		throw Parser::InvalidParam(err, *this);
	}
}

void Parser::isValidIPv6(const std::string& ip) const {
	
	std::istringstream iss(ip);
	std::string block;
	std::string err("Invalid IP address " + ip);
	int count = 0;
	int doubleColonCount = 0;

	for (std::string::const_iterator it = ip.begin(); it != ip.end(); ++it) {
		if (*it == ':') {
			if (it + 1 != ip.end() && *(it + 1) == ':') {
				++doubleColonCount;
			}
		}
	}

	if (doubleColonCount > 1) {
		throw Parser::InvalidParam(err, *this);
	}
	while (std::getline(iss, block, ':')) {
		
		++count;

		if (count > 8) {
			throw Parser::InvalidParam(err, *this);
		}
		if (block.empty()) {
			continue;
		}
		if (block.length() > 4) {
			throw Parser::InvalidParam(err, *this);
		}

		for (std::string::const_iterator it = block.begin(); it != block.end(); ++it) {
			char c = std::tolower(*it);
			if (!std::isdigit(c) && (c < 'a' || c > 'f')) {
				throw Parser::InvalidParam(err, *this);
			}
		}
	}

	if (count < 8 && doubleColonCount == 0) {
		throw Parser::InvalidParam(err, *this);
	}
}

bool	Parser::isCommentOrEmptyLine(std::string const& line) const {

	return line.empty() or line.find_first_not_of(" ") == line.find('#') 
		or line.find_first_not_of(" ") == std::string::npos;
}

bool	Parser::isDirective(std::string const& line) const {

	size_t	semicolonPos;

	semicolonPos = line.find_first_of(';');
	return 	semicolonPos != std::string::npos 
			&& line.find_first_not_of(" ", semicolonPos) != line.find('\n');
}

bool	Parser::isEndOfScope(std::string const& line) const {

	return line.find('}') != std::string::npos 
		and line.find_first_not_of("} ") == std::string::npos;
}



void	Parser::trimAndReplaceWhitespaces(std::string& input) {

	std::stringstream	ss(input);
	std::string			word;

	input.clear();
	while (ss >> word) {
		if (not input.empty()) {
			input += " ";
		}
		input += word;
	}
}

void	Parser::buildAndThrowParamError(std::string const& line) const {

	std::string param = line.substr(0, line.find_first_of(" "));
	param.substr(0, param.find_first_of(";"));
	std::string err = "Error: Invalid parameter '" + param + "'";
	throw Parser::InvalidParam(err, *this);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: EXCEPTIONS::

Parser::Error::Error(std::string msg) : _msg(msg) {}

Parser::Error::~Error() throw() {}

const char* Parser::Error::what() const throw() { return _msg.c_str(); }

Parser::InvalidParam::InvalidParam(std::string const& err, Parser const &p) : Error("") { 

	std::stringstream stream;

	stream << "neoserv: " << err << " in " + p.getConfFileName() + ":";
	stream << p.getLinesRead() << std::endl;
	stream << "neoserv: configuration file " << p.getConfFileName() << " test failed" << std::endl;

	_msg = stream.str();
}

Parser::InvalidParam::~InvalidParam() throw() {}