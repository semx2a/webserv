#include <iostream>
#include "CommonConfig.hpp"
#include "Config.hpp"
#include "Parser.hpp"
#include "SpecConfig.hpp"
#include "print.hpp"

void	DisplayParserContents(Parser const& parser) {

	std::cout << std::endl;
	std::cout << HIORANGE << "--- Parser ---" << NO_COLOR << std::endl;
	std::cout << HIPURPLE << "linesRead: " << NO_COLOR << parser.getLinesRead() << std::endl;
	std::cout << HIPURPLE << "confFilename: " << NO_COLOR << parser.getConfFileName() << std::endl;
}

int	main() {

	Config 	config;
	//Parser const &					parser = config.getParser();

	DisplayParserContents(config.getParser());
	std::cout << config.getParser().getCommonConfig() << std::endl;
	std::cout << print_vector(config.getParser().getSpecConfigs()) << std::endl;

	return 0;
}
