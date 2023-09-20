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
	while (std::getline(stream, line, 'n')) {

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
		if (line.find("location") != std::string::npos) {
			// find location, in another function that getline until the closing bracket
		}
		else if (line.find(';') != line.find('\n') - 1)
		{
			if (line.find('}') != std::string::npos && line.find_first_not_of("} \t") == std::string::npos) {
				//end of server config
				return;
			}	
			else if (line.find("listen") != std::string::npos) {
				// set listen
			}
			else if (line.find("server_name") != std::string::npos) {
				// set server name(s)
			}
			else if (line.find("error_page") != std::string::npos) {
				// set error page(s)
			}
			else if (line.find("root") != std::string::npos) {
				// set root
			}
		}
		else if (line.empty () || line.find_first_not_of("\t ") == line.find('#')) {
			// empty line or comment
		}
		else {
			std::string param = line.substr(0, line.find_first_of(" \t"));
			param.substr(0, param.find_first_of(";"));
			throw Parser::InvalidParam(param, _confFilename, _linesRead);
		}
	}
	this->_specConfigs.push_back(server);
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