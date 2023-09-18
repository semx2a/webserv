# include "Parser.hpp"

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORRS::

Parser::Parser (std::string const& conf_filename) : _CommonConfig(), _SpecConfigs() {

	try {
		parse(conf_filename);
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
	}
	return *this;
}

Parser::~Parser(void) {}	

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

CommonConfig const &				Parser::getCommonConfig(void) const { return this->_CommonConfig; }

std::vector<SpecConfig> const &	Parser::getSpecConfigs(void) const { return this->_SpecConfigs; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void	Parser::parse(std::string const& conf_filename) {

	std::ifstream		file(conf_filename.c_str());
	std::stringstream	stream;

	file.open(conf_filename.c_str());
	if (!file.is_open())
		throw Parser::Error("Error: Cannot open file " + conf_filename);
	
	stream << file.rdbuf();

	std::string line;
	while (std::getline(stream, line, 'n')) {

		if (*line.begin() == '#')
			continue;
		if (line.find("server") != std::string::npos) {
			// SpecConfig
		}
		else {
			// CommonConfig
		}
	}
	
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: EXCEPTIONS::

Parser::Error::Error(std::string const& msg) : _msg(msg) {}

Parser::Error::~Error() throw() {}

const char* Parser::Error::what() const throw() { return _msg.c_str(); }