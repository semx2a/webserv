#include <iostream>
#include "ServerContext.hpp"
#include "Config.hpp"
#include "Parser.hpp"
#include "print.hpp"

void	DisplayParserContents(Parser const& parser) {

	std::cout << std::endl;
	std::cout << RED << "--- Parser ---" << NO_COLOR << std::endl;
	std::cout << HIPURPLE << std::setw(21) << "linesRead: " << HIGREEN << parser.linesRead() << NO_COLOR << std::endl;
	std::cout << HIPURPLE << std::setw(21) << "confFilename: " << HIGREEN << parser.confFileName() << NO_COLOR << std::endl;
}

int	main() {

	Config 							config;
	Parser const &					parser = config.parser();

	DisplayParserContents(parser);

	std::cout << std::endl;
	std::cout << RED << "--- ServerContexts ---" << NO_COLOR << std::endl;
	std::cout << print_classVector(parser.serverContexts()) << std::endl;

	return 0;
}
