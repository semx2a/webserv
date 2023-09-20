# include "Parser.hpp"

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORRS::

Parser::Parser() : _linesRead(0), _confFilename(), _commonConfig(), _specConfigs() {}

Parser::Parser (std::string const& _conf_filename) :  _linesRead(0), _confFilename(_conf_filename), _commonConfig(), _specConfigs()  {

	try {
		parse();
		std::cout << "neoserv: the configuration file " << this->_confFilename << " syntax is ok" << std::endl;
		std::cout << "neoserv: configuration file " << this->_confFilename << " test is successful" << std::endl;
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

Parser::Parser(Parser const &rhs) : _linesRead(rhs.getLinesRead()), _confFilename(rhs.getConfFileName()), _commonConfig(), _specConfigs() {
	*this = rhs;
}

Parser &	Parser::operator=(Parser const & rhs) {

	if (this != &rhs) {

		this->_linesRead = rhs.getLinesRead();
		this->_confFilename = rhs.getConfFileName();
		this->_commonConfig = rhs.getCommonConfig();
		this->_specConfigs = rhs.getSpecConfigs();
	}
	return *this;
}

Parser::~Parser(void) {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

CommonConfig const &				Parser::getCommonConfig(void) const { return this->_commonConfig; }

std::vector<SpecConfig> const &		Parser::getSpecConfigs(void) const { return this->_specConfigs; }

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
	while (std::getline(stream, line, '\n')) {

		this->_linesRead++;
		if (line.find("server {") != std::string::npos) {
			parseSpecConfig(stream);
		}
		//else if line with config outside the scope
		else if (line.find_first_not_of("\t\n ") <= line.find('#'))
			continue;
		else {
			throw Parser::Error("Error: Invalid config file");
		}
	}
}

void	Parser::parseSpecConfig(std::stringstream& stream) {

	SpecConfig 	server;
	std::string line;

	while (std::getline(stream, line, '\n')) {

		this->_linesRead++;
		if (line.find("location") != std::string::npos) {
			// find location, in another function that getline until the closing bracket
		}
		else if (line.find(';') != line.find('\n') - 1)
		{
			if (line.find('}') != std::string::npos && line.find_first_not_of("} \t") == std::string::npos) {
				break ;
			}	
			else if (line.find("listen") != std::string::npos) {
				this->parseListen(line, server);
				//this->isValidIPv6(server.getListenIpPort().begin()->first);
			}
			else if (line.find("server_name") != std::string::npos) {
				server.setServerName(line);
			}
			else if (line.find("error_page") != std::string::npos) {
				server.setErrorPage(line);
			}
			else if (line.find("root") != std::string::npos) {
				server.setRoot(line);
			}
		}
		else if (line.empty () || line.find_first_not_of("\t ") == line.find('#')) {
			continue ;
		}
		else {
			std::string param = line.substr(0, line.find_first_of(" \t"));
			param.substr(0, param.find_first_of(";"));
			std::string err = "Error: Invalid parameter " + param;
			throw Parser::InvalidParam(err, *this);
		}
	}	
	this->_specConfigs.push_back(server);
}

void	Parser::parseListen(std::string const& line, SpecConfig& server) {

	std::stringstream	stream(line);
	std::string			listen;
	std::string			ip;
	int					port(80); //If only address is given, the port 80 is used.

//	std::map<std::string, int>::iterator it = this->_listenIpPort.begin();
//	if (it->first == "127.0.0.1" && it->second == 80)
//		this->_listenIpPort.erase(it);

	stream >> listen;
	if (stream.str().find(':') != std::string::npos)
	{
		std::getline(stream, ip, ':');
		ip = ip.substr(ip.find_first_not_of(" \t"), ip.size());
		this->isValidIPv4(ip);
	}
	stream >> port;
	server.setListIpPort(ip, port);
}

void Parser::parseServerName(std::string const& line, SpecConfig& server) {

	std::stringstream	stream(line);
	std::string			tmp;
	std::string			serverName;

//	std::vector<std::string>::iterator it = server.getServerNames().begin();
//	if (*it == "localhost")
//		server.getServerNames().erase(it);
	
	stream >> tmp >> serverName;
	serverName = serverName.substr(0, serverName.find_first_of(";"));
	server.setServerName(serverName);
}

void Parser::isValidIPv4(const std::string& ip) {
	
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

void Parser::isValidIPv6(const std::string& ip) {
	
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