# include "Parser.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Parser::Parser() : _linesRead(0), _confFilename(), _serverContexts() {}

Parser::Parser (std::string const& _conf_filename) :  _linesRead(0), _confFilename(_conf_filename), _serverContexts() {

	try {
		parse();
		std::cout << "neoserv: the configuration file " << this->_confFilename << " syntax is ok" << std::endl;
		std::cout << "neoserv: configuration file " << this->_confFilename << " test is successful" << std::endl;
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

Parser::Parser(Parser const &rhs) {
	*this = rhs;
}

Parser &	Parser::operator=(Parser const & rhs) {

	if (this != &rhs) {

		this->_linesRead = rhs.getLinesRead();
		this->_confFilename = rhs.getConfFileName();
		this->_serverContexts = rhs.getServerContexts();
	}
	return *this;
}

Parser::~Parser(void) {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

std::vector<ServerContext> const &		Parser::getServerContexts(void) const { return this->_serverContexts; }

std::string const &					Parser::getConfFileName(void) const { return this->_confFilename; }

size_t								Parser::getLinesRead(void) const { return this->_linesRead; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void	Parser::parse() {

	std::ifstream		file(_confFilename.c_str());
	std::stringstream	stream;

	file.open(_confFilename.c_str());
	if (not file.is_open())
		throw Parser::Error("Error: Cannot open file " + _confFilename);
	
	stream << file.rdbuf();

	std::string line;
	while (std::getline(stream, line)) {

		this->_linesRead++;
		this->trimAndReplaceWhitespaces(line);

		#ifdef DEBUG
		std::cout << "[main scope] line " << _linesRead << ": " << line << std::endl;
		#endif

		if (isCommentOrEmptyLine(line)) {
			continue;
		}
		if (isEndOfScope(line)) {
			break;
		}
		else if (line.find("server {") != std::string::npos) {
			this->parseServerContext(stream);
		}
		else {
			this->buildAndThrowParamError(line);
		}
	}
}

void Parser::parseServerContext(std::stringstream& stream) {

	Parser::DirectivePair directiveArray[] = {

		Parser::DirectivePair("autoindex", &Parser::parseAutoindex),
		Parser::DirectivePair("cgi", &Parser::parseCgi<ServerContext>),
		Parser::DirectivePair("client_max_body_size", &Parser::parseMaxBodySize<ServerContext>),
		Parser::DirectivePair("root", &Parser::parseRoot<ServerContext>),
		Parser::DirectivePair("listen", &Parser::parseListen),
		Parser::DirectivePair("error_page", &Parser::parseErrorPage<ServerContext>),
		Parser::DirectivePair("index", &Parser::parseIndex),
		Parser::DirectivePair("authorized_methods", &Parser::parseAuthorizedMethods<ServerContext>),
		Parser::DirectivePair("server_name", &Parser::parseServerName)
	};

	// initialization of the map with the content of the array of pairs
	std::map<std::string, ParserFunction> directiveMap(directiveArray, 
														directiveArray + sizeof(directiveArray) / sizeof(DirectivePair));

	ServerContext	newServerCtxt;
	std::string		line;

	while (std::getline(stream, line)) {

		this->_linesRead++;
		this->trimAndReplaceWhitespaces(line);

		#ifdef DEBUG
		std::cout << "[server scope] line " << _linesRead << ": " << line << std::endl;
		#endif

		if (isCommentOrEmptyLine(line)) {
			continue;
		}
		if (isEndOfScope(line)) {
			break;
		}
		if (line.find("location") != std::string::npos) {
			this->parseServerLocationContext(stream, newServerCtxt);
		} 
		else if (this->isDirective(line)) {

			line = line.substr(0, line.find_first_of(";"));

			std::string directive = line.substr(0, line.find_first_of(" "));
			Parser::DirectiveMap::iterator it = directiveMap.find(directive);
			if (it != directiveMap.end()) {
				(this->*(it->second))(line, newServerCtxt);
			} 
			else {
				this->buildAndThrowParamError(line);
			}
		}
		else {
			buildAndThrowParamError(line);
		}
	}
	std::cout << "newServerCtxt.getCgi() = " << newServerCtxt.getCgi() << std::endl;
	this->_serverContexts.push_back(newServerCtxt);
}


void	Parser::parseServerLocationContext(std::stringstream& stream, ServerContext& serverContext) {

	//TODO
	(void)serverContext;
	std::string 	line;

	while (std::getline(stream, line)) {

		this->_linesRead++;
		this->trimAndReplaceWhitespaces(line);

		#ifdef DEBUG
		std::cout << "[location scope] line " << _linesRead << ": " << line << std::endl;
		#endif

		if (isEndOfScope(line))
			return;
	}
}

void	Parser::parseAutoindex(std::string const &line, ServerContext& serverContext) {

	std::stringstream	stream(line);
	std::string			directive;
	std::string			onOff;

	stream >> directive >> onOff;
	if (onOff != "on" and onOff != "off")
		buildAndThrowParamError(line);

	serverContext.setAutoindex(onOff);
}

template<typename Context>
void	Parser::parseCgi(std::string const& line, Context& context) {

	std::stringstream	stream(line);
	std::string			directive;
	std::string			onOff;

	std::cout << "context.getCgi() = " << context.getCgi() << std::endl;
	if (not(stream >> directive >> onOff) or not stream.eof()) {
		buildAndThrowParamError(line);
	}
	if (onOff != "on" and onOff != "off")
		buildAndThrowParamError(line);

	std::cout << "onOff : " << onOff << std::endl;
	context.setCgi(onOff);
	std::cout << "context.getCgi() = " << context.getCgi() << std::endl;
}

template<typename Context>
void	Parser::parseMaxBodySize(std::string const& line, Context& context) {

	std::stringstream	stream(line);
	std::string 		directive;
	std::string			sizeStr;

	if (not(stream >> directive >> sizeStr) or not stream.eof()) {
		buildAndThrowParamError(line);
	}

	size_t mPos = sizeStr.find_first_of("mM");
	if (sizeStr.find_first_not_of("0123456789") != mPos or sizeStr.find_first_not_of(" ", mPos + 1) != std::string::npos) {
		buildAndThrowParamError(line);
	}

	int size = std::atoll(sizeStr.substr(0, mPos).c_str());
	size *= 1e6;
	context.setMaxBodySize(size);
}

template<typename Context>
void	Parser::parseRoot(std::string const& line, Context& context) {

	std::stringstream stream(line);
	std::string directive;
	std::string root;

	if (not (stream >> directive >> root) or not stream.eof()) {
		buildAndThrowParamError(line);
	}

	context.setRoot(root);
}

void	Parser::parseListen(std::string const& line, ServerContext& serverContext) {

	std::stringstream	stream(line);
	std::string			listen;
	std::string			ip;
	int					port(80); //If only address is given, the port 80 is used.

//	std::map<std::string, int>::iterator it = this->_listen.begin();
//	if (it->first == "127.0.0.1" and it->second == 80)
//		this->_listen.erase(it);

//	TODO : check with defaut & possible values

	stream >> listen;
	if (stream.str().find(':') != std::string::npos)
	{
		std::getline(stream, ip, ':');
		if (ip.empty())
			ip = "127.0.0.1";
		ip = ip.substr(ip.find_first_not_of(" "), ip.size());
		this->isValidIPv4(ip);
	}
	stream >> port;
	serverContext.setListen(ip, port);
}

template<typename Context>
void	Parser::parseErrorPage(std::string const& line, Context& context) {

	std::stringstream	stream(line);
	std::string			directive;
	std::string			errorCode;
	std::string			errorPage;

	stream >> directive >> errorCode >> errorPage;
	std::cout << "Error code : " << errorCode << std::endl;
	if (errorCode.find_first_not_of("0123456789") != std::string::npos) {
		this->buildAndThrowParamError(line);
	}
	context.addErrorPage(std::atoi(errorCode.c_str()), errorPage);
}

void	Parser::parseIndex(std::string const &line, ServerContext& serverContext) {

    std::stringstream            stream(line);
    std::string                  index;
    std::vector<std::string>     indexVec;

    stream >> index;
    while (stream >> index) {

        if (index.find(';') != std::string::npos)
            index = index.substr(0, index.find_first_of(";"));
        indexVec.push_back(index);
    }
    if (indexVec.empty())
        indexVec.push_back("index.html");
    serverContext.setIndex(indexVec);
}

template<typename Context>
void	Parser::parseAuthorizedMethods(const std::string& line, Context& context) {

	std::stringstream			stream(line);
	std::vector<std::string>	authorizedMethods;
	std::string					directive;
	std::string					method;

	stream >> directive;
	while (stream >> method) {
		authorizedMethods.push_back(method);
	}
	context.setAuthorizedMethods(authorizedMethods);
}

void Parser::parseServerName(std::string const& line, ServerContext& serverContext) {

	std::stringstream			stream(line);
	std::string					directive;
	std::string					serverName;
	std::vector<std::string> 	serverNames;

//	std::vector<std::string>::iterator it = serverContext.getServerNames().begin();
//	if (*it == "localhost")
//		serverContext.getServerNames().erase(it);
	
	stream >> directive;
	while (stream >> serverName) {
		if (serverName.find(';') != std::string::npos)
			serverName = serverName.substr(0, serverName.find_first_of(";"));
		serverNames.push_back(serverName);
	}
	serverContext.setServerNames(serverNames);
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
			if (not std::isdigit(*it)) {
				throw Parser::InvalidParam(err, *this);
			}
		}

		int num = std::atoi(octet.c_str());
		if (num < 0 or num > 255) {
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
			if (it + 1 != ip.end() and *(it + 1) == ':') {
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
			if (not std::isdigit(c) and (c < 'a' or c > 'f')) {
				throw Parser::InvalidParam(err, *this);
			}
		}
	}

	if (count < 8 and doubleColonCount == 0) {
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
			and line.find_first_not_of(" ", semicolonPos) != line.find('\n');
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