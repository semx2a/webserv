#include <iostream>
#include "ServerContext.hpp"
#include "Parser.hpp"
#include "utl.hpp"

void	DisplayParserContents(Parser const& parser) {

	std::cout << std::endl;
	std::cout << RED << "--- Parser ---" << RESET << std::endl;
	std::cout << HIPURPLE << std::setw(21) << "linesRead: " << HIGREEN << parser.linesRead() << RESET << std::endl;
	std::cout << HIPURPLE << std::setw(21) << "confFilename: " << HIGREEN << parser.confFileName() << RESET << std::endl;
}

int	main(int ac, char **av) {

	if (ac > 2)
		return 0;
	std::string confFile = (ac == 2) 
								? av[1]
								: "../neoserv/conf/default.conf";
	Parser const &	parser(confFile);

	DisplayParserContents(parser);

	std::cout << std::endl;
	std::cout << RED << "--- ServerContexts ---" << RESET << std::endl;
	std::cout << utl::print_classVector(parser.serverContexts()) << std::endl;

	return 0;
}
