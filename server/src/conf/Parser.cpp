# include "Parser.hpp"

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORRS::

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
	if (!file.is_open())
		throw Parser::Error("Error: Cannot open file " + _confFilename);
	
	stream << file.rdbuf();

	std::string line;
	while (std::getline(stream, line)) {

		this->_linesRead++;
		this->trimAndReplaceWhitespaces(line);
		std::cout << "[main scope] line " << _linesRead << ": " << line << std::endl;
		if (line.find("server {") != std::string::npos)
			this->parseServerContext(stream);
		else if (isCommentOrEmptyLine(line))
			continue;
		else 
			this->buildAndThrowParamError(line);
	}
}

void Parser::parseServerContext(std::stringstream& stream) {

    Parser::DirectivePair directiveArray[] = {
        Parser::DirectivePair("autoindex", &Parser::parseAutoindex),
        Parser::DirectivePair("client_max_body_size", &Parser::parseClientMaxBodySize),
        Parser::DirectivePair("root", &Parser::parseRoot),
        Parser::DirectivePair("listen", &Parser::parseListen),
        Parser::DirectivePair("error_page", &Parser::parseErrorPage),
        Parser::DirectivePair("index", &Parser::parseIndex),
        Parser::DirectivePair("authorized_methods", &Parser::parseAuthorizedMethods),
        Parser::DirectivePair("server_name", &Parser::parseServerName)
    };

	// initialization of the map with the content of the array of pairs
    std::map<std::string, ParserFunction> directiveMap(directiveArray, 
														directiveArray + sizeof(directiveArray) / sizeof(DirectivePair));

    ServerContext newServerCtxt;
    std::string line;

    while (std::getline(stream, line)) {

        this->_linesRead++;
        this->trimAndReplaceWhitespaces(line);

        std::cout << "[server scope] line " << _linesRead << ": " << line << std::endl;

        if (isCommentOrEmptyLine(line)) continue;
        if (isEndOfScope(line)) break;

	    if (line.find("location") != std::string::npos) {
            this->parseServerLocationContext(stream, newServerCtxt);
        } 
        else if (this->isValidDirective(line)) {
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
    this->_serverContexts.push_back(newServerCtxt);
}


void	Parser::parseServerLocationContext(std::stringstream& stream, ServerContext& serverContext) {

	//TODOOOO
	(void)serverContext;
	std::string 	line;

	while (std::getline(stream, line)) {

		this->_linesRead++;
		this->trimAndReplaceWhitespaces(line);
		std::cout << "[location scope] line " << _linesRead << ": " << line << std::endl;
		if (isEndOfScope(line))
			return;
	}
}

void	Parser::parseListen(std::string const& line, ServerContext& serverContext) {

	std::stringstream	stream(line);
	std::string			listen;
	std::string			ip;
	int					port(80); //If only address is given, the port 80 is used.

//	std::map<std::string, int>::iterator it = this->_listen.begin();
//	if (it->first == "127.0.0.1" && it->second == 80)
//		this->_listen.erase(it);

	stream >> listen;
	if (stream.str().find(':') != std::string::npos)
	{
		std::getline(stream, ip, ':');
		ip = ip.substr(ip.find_first_not_of(" "), ip.size());
		this->isValidIPv4(ip);
	}
	stream >> port;
	if (!ip.empty() && port != 80)
		serverContext.setListen(ip, port);
}

void Parser::parseServerName(std::string const& line, ServerContext& serverContext) {

	std::stringstream			stream(line);
	std::string					tmp;
	std::string					serverName;
	std::vector<std::string> 	serverNames;

//	std::vector<std::string>::iterator it = serverContext.getServerNames().begin();
//	if (*it == "localhost")
//		serverContext.getServerNames().erase(it);
	
	stream >> tmp;
	while (stream >> serverName) {
		if (serverName.find(';') != std::string::npos)
			serverName = serverName.substr(0, serverName.find_first_of(";"));
		serverNames.push_back(serverName);
	}
	serverContext.setServerNames(serverNames);
}

void	Parser::parseClientMaxBodySize(std::string const& line, ServerContext& serverContext) {
	
	std::stringstream	stream(line);
	std::string			directive;
	std::string			sizeStr;
	size_t				mPos;
	size_t				size;

	stream >> directive >> sizeStr;
	mPos = sizeStr.find_first_of("mM");
	if (sizeStr.find_first_not_of("0123456789") != mPos 
		|| sizeStr.find_first_not_of(" ", mPos + 1) != std::string::npos) {
		throw Parser::InvalidParam("Error: Invalid parameter " + sizeStr, *this);
	}
	sizeStr = sizeStr.substr(0, sizeStr.find_first_of("mM"));
	size = std::atoll(sizeStr.c_str());
	size *= 1000000;
	serverContext.setClientMaxBodySize(size);
}

void	Parser::parseErrorPage(std::string const &line, ServerContext& serverContext) { 

	(void)line;
	(void)serverContext;

//	std::stringstream	stream(line);
//	std::string			tmp;
//
//	//todo
}

void	Parser::parseIndex(std::string const &line, ServerContext& serverContext) {

	(void)line;
	(void)serverContext;
//	std::stringstream	stream(line);
//	std::string			tmp;
//
//	stream >> tmp;
//	while (stream >> tmp) {
//		this->_index.push_back(tmp);
//	}
}

void	Parser::parseRoot(std::string const &line, ServerContext& serverContext) { 

	(void)line;
	(void)serverContext;
//	std::stringstream	stream(line);
//	std::string			tmp;
//
//	stream >> tmp >> this->_root;
}

void	Parser::parseAutoindex(std::string const &line, ServerContext& serverContext) {

	(void)line;
	(void)serverContext;

//	find on/off
}

void	Parser::parseAuthorizedMethods(std::string const& line, ServerContext& serverContext) { 

	(void)line;
	(void)serverContext;
//	std::stringstream	stream(line);
//	std::string			tmp;
//
//	stream >> tmp;
//	while (stream >> tmp) {
//		this->_authorizedMethods.push_back(tmp);
//	}
}

void	Parser::parseLocation(std::stringstream& stream, ServerContext& serverContext) { 
	
	(void)stream;
	(void)serverContext;
//	go through lines until closing bracket
//	this->_locations[location] = path;


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

	return line.empty() || line.find_first_not_of(" ") == line.find('#') 
		|| line.find_first_not_of(" ") == std::string::npos;
}

bool	Parser::isValidDirective(std::string const& line) const {

	size_t	semicolonPos;

	semicolonPos = line.find_first_of(';');
	return 	semicolonPos != std::string::npos 
			&& line.find_first_not_of(" ", semicolonPos) != line.find('\n');
}

bool	Parser::isEndOfScope(std::string const& line) const {

	return line.find('}') != std::string::npos 
		&& line.find_first_not_of("} ") == std::string::npos;
}



void	Parser::trimAndReplaceWhitespaces(std::string& input) {

	std::stringstream	ss(input);
	std::string			word;

	input.clear();
	while (ss >> word) {
		if (!input.empty()) {
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