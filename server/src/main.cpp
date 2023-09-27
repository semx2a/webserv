#include "Parser.hpp"
#include "Engine.hpp"

int	main(int ac, char **av) {

	try {

		if (ac > 2)
			throw std::invalid_argument("Usage: ./webserv <config_file>");

		std::string conf_file;
		conf_file = (ac == 1) ? "../neoserv/conf/default.conf" : av[1];
		
		Parser	parser(conf_file);
		Engine	webserv (parser.getServerContexts());
		webserv.connect();
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	return 0;
}
