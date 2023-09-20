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

		_linesRead++;
		std::cout << _linesRead << " " << line << std::endl;
		if (line.find("location") != std::string::npos) {
			// find location, in another function that getline until the closing bracket
		}
		else if (line.find(';') != line.find('\n') - 1)
		{
			if (line.find('}') != std::string::npos && line.find_first_not_of("} \t") == std::string::npos) {
				std::cout << "end of server scope" << std::endl;
				break ;
			}	
			else if (line.find("listen") != std::string::npos) {
				std::cout << "listen" << std::endl;
				server.setListIpPort(line);
			}
			else if (line.find("server_name") != std::string::npos) {
				std::cout << "serverName" << std::endl;
				server.setServerName(line);
			}
			else if (line.find("error_page") != std::string::npos) {
				std::cout << "errorPage" << std::endl;
				server.setErrorPage(line);
			}
			else if (line.find("root") != std::string::npos) {
				std::cout << "root" << std::endl;
				server.setRoot(line);
			}
		}
		else if (line.empty () || line.find_first_not_of("\t ") == line.find('#')) {
			continue ;
		}
		else {
			std::string param = line.substr(0, line.find_first_of(" \t"));
			param.substr(0, param.find_first_of(";"));
			throw Parser::InvalidParam(param, _confFilename, _linesRead);
		}
	}	
	this->_specConfigs.push_back(server);
}

bool Parser::isValidIPv4(const std::string& str) {
	
	std::istringstream iss(str);
	std::string octet;
	int count = 0;

	while (std::getline(iss, octet, '.')) {
		++count;

		if (count > 4) {
			return false;
		}

		for (std::string::const_iterator it = octet.begin(); it != octet.end(); ++it) {
			if (!std::isdigit(*it)) {
				return false;
			}
		}

		int num = std::atoi(octet.c_str());
		if (num < 0 || num > 255) {
			return false;
		}
	}

	if (count != 4) {
		return false;
	}

	return true;
}

bool Parser::isValidIPv6(const std::string& str) {
	
	std::istringstream iss(str);
	std::string block;
	int count = 0;
	int doubleColonCount = 0;

	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (*it == ':') {
			if (it + 1 != str.end() && *(it + 1) == ':') {
				++doubleColonCount;
			}
		}
	}

	if (doubleColonCount > 1) {
		return false;
	}
	while (std::getline(iss, block, ':')) {
		
		++count;

		if (count > 8) {
			return false;
		}
		if (block.empty()) {
			continue;
		}
		if (block.length() > 4) {
			return false;
		}

		for (std::string::const_iterator it = block.begin(); it != block.end(); ++it) {
			char c = std::tolower(*it);
			if (!std::isdigit(c) && (c < 'a' || c > 'f')) {
				return false;
			}
		}
	}

	if (count < 8 && doubleColonCount == 0) {
		return false;
	}

	return true;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: EXCEPTIONS::

Parser::Error::Error(std::string msg) : _msg(msg) {}

Parser::Error::~Error() throw() {}

const char* Parser::Error::what() const throw() { return _msg.c_str(); }

Parser::InvalidParam::InvalidParam(std::string const& param, std::string const& confFilename, size_t linesRead) : Error("") { 

	std::stringstream stream;

	stream << "invalid parameter \"" << param + " in " + confFilename + ":";
	stream << linesRead << std::endl;
	stream << "neoserv: configuration file " << confFilename << " test failed" << std::endl;

	_msg = stream.str();
}

Parser::InvalidParam::~InvalidParam() throw() {}