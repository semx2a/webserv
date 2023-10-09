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

		this->_linesRead = rhs.linesRead();
		this->_confFilename = rhs.confFileName();
		this->_serversContexts = rhs.serverContexts();
	}
	return *this;
}

Parser::~Parser(void) {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

std::vector<ServerContext> const&	Parser::serverContexts(void) const { return this->_serversContexts; }

std::string const&					Parser::confFileName(void) const { return this->_confFilename; }

size_t								Parser::linesRead(void) const { return this->_linesRead; }

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
		line = utl::trimAndReplaceWs(line);

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
	checkIfPortDoublons();
}


Parser::DirectiveMap<ServerContext>::type Parser::initializeServerDirectives() {
    Parser::DirectiveMap<ServerContext>::type directiveMap;

	directiveMap["autoindex"] = &Parser::parseAutoindex<ServerContext>;
	directiveMap["client_max_body_size"] = &Parser::parseMaxBodySize<ServerContext>;
	directiveMap["root"] = &Parser::parseRoot<ServerContext>;
    directiveMap["listen"] = &Parser::parseListen<ServerContext>;
	directiveMap["error_page"] = &Parser::parseErrorPage<ServerContext>;
	directiveMap["index"] = &Parser::parseIndex<ServerContext>;
	directiveMap["authorized_methods"] = &Parser::parseAuthorizedMethods<ServerContext>;
    directiveMap["server_name"] = &Parser::parseServerName<ServerContext>;

    return directiveMap;
}

Parser::DirectiveMap<LocationContext>::type Parser::initializeLocationDirectives() {
    Parser::DirectiveMap<LocationContext>::type directiveMap;

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

void	Parser::searchIfCgi(LocationContext& locationContext, std::string& path) {

	if (path != "*.py" || path != ".php")
		return;
	locationContext.setIsCgi(true);
	path = path.substr(path.find_first_of("*") + 1, path.size() - path.find_first_of("*") - 1);
}

void	Parser::buildAndThrowParamError(std::string const& line) const {

	std::string param = line.substr(0, line.find_first_of(" "));
	param.substr(0, param.find_first_of(";"));
	std::string err = "Invalid parameter '" + param + "'";
	throw Parser::InvalidParam(err, *this);
}

void	Parser::checkIfPortDoublons() {

	std::set <int> setPorts;
	for (std::vector<ServerContext>::const_iterator serversIt = _serversContexts.begin(); serversIt != _serversContexts.end(); serversIt++) {
		for (std::map<std::string, int>::const_iterator ipPortIt = serversIt->listen().begin(); ipPortIt != serversIt->listen().end(); ipPortIt++) {
			
			if (setPorts.find(ipPortIt->second) != setPorts.end()) {
				std::stringstream stream;
				stream << "neoserv: port " << ipPortIt->second << " already in use" << std::endl;
				throw Parser::Error(stream.str());
			}
			setPorts.insert(ipPortIt->second);
		}
	}
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: EXCEPTIONS::

Parser::Error::Error(std::string msg) : _msg(msg) {}

Parser::Error::~Error() throw() {}

const char* Parser::Error::what() const throw() { return _msg.c_str(); }

Parser::InvalidParam::InvalidParam(std::string const& err, Parser const &p) : Error("") { 

	std::stringstream stream;

	stream << "neoserv: " << err << " in " + p.confFileName() + ":";
	stream << p.linesRead() << std::endl;
	stream << "neoserv: configuration file " << p.confFileName() << " test failed" << std::endl;

	_msg = stream.str();
}

Parser::InvalidParam::~InvalidParam() throw() {}