#include "Parser.hpp"
#include "Engine.hpp"

int	main(int ac, char **av) {

	std::cout << std::endl;
	try {

		if (ac > 2)
			throw std::invalid_argument("Usage: ./webserv <config_file>");

		std::string conf_file;
		conf_file = (ac == 1) ? "../neoserv/conf/testwebsite.conf" : av[1];
		
		Parser	parser(conf_file);
		Engine	webserv (parser.serverContexts());
		webserv.connexionLoop();
	}
	catch (Parser::Error const& e) {
		std::cerr << BOLD << "Error: " << RESET << e.what() << std::endl;
	}
	catch (std::exception const& e) {
		std::cerr << BOLD << "Fatal: " << RESET << e.what() << std::endl;
	}
	return 0;
}
