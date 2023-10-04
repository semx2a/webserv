#include <iostream>
#include "ServerContext.hpp"
#include "Config.hpp"
#include "Parser.hpp"
#include "utl.hpp"

void	DisplayParserContents(Parser const& parser) {

	std::cout << std::endl;
	std::cout << RED << "--- Parser ---" << RESET << std::endl;
	std::cout << HIPURPLE << std::setw(21) << "linesRead: " << HIGREEN << parser.linesRead() << RESET << std::endl;
	std::cout << HIPURPLE << std::setw(21) << "confFilename: " << HIGREEN << parser.confFileName() << RESET << std::endl;
}

int	main() {

	Config 							config;
	Parser const &					parser = config.parser();

	DisplayParserContents(parser);

	std::cout << std::endl;
	std::cout << RED << "--- ServerContexts ---" << RESET << std::endl;
	std::cout << utl::print_classVector(parser.serverContexts()) << std::endl;

	return 0;
}
