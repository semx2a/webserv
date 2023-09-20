#include <iostream>
#include "CommonConfig.hpp"
#include "Config.hpp"
#include "Parser.hpp"
#include "SpecConfig.hpp"
#include "print.hpp"

void	DisplayParserContents(Parser const& parser) {

	std::cout << std::endl;
	std::cout << RED << "--- Parser ---" << NO_COLOR << std::endl;
	std::cout << HIPURPLE << "linesRead: " << HIGREEN << parser.getLinesRead() << NO_COLOR << std::endl;
	std::cout << HIPURPLE << "confFilename: " << HIGREEN << parser.getConfFileName() << NO_COLOR << std::endl;
}

int	main() {

	Config 							config;
	Parser const &					parser = config.getParser();

	DisplayParserContents(parser);

	std::cout << std::endl;
	std::cout << RED << "--- CommonConfig ---" << NO_COLOR << std::endl;
	std::cout << parser.getCommonConfig() << std::endl;

	std::cout << RED << "--- SpecConfig ---" << NO_COLOR << std::endl;
	std::cout << print_classVector(parser.getSpecConfigs()) << std::endl;

	return 0;
}
