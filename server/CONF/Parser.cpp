# include "Parser.hpp"

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORRS::

Parser::Parser (std::string const& CommonConfig_file) : _CommonConfig(), _SpecConfigs() {

	try {
		parse(CommonConfig_file);
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

void	Parser::parse(std::string const& CommonConfig_file) {

	std::ifstream		file(CommonConfig_file.c_str());
	std::stringstream	stream;

	file.open(CommonConfig_file.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: Cannot open file " + CommonConfig_file);
	
	stream << file.rdbuf();
	file.close();

	std::string line;
	
	while (std::getline(stream, line, '\n')) {

		if (scope == false)
		{
			if  (line == "server") {
				
			}
		}
	}
	
}

