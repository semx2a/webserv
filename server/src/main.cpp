//#include "../inc/Webserv.hpp"

//const int PORT = 8080;
//
//int main() {
//	
//    Webserv server(PORT);
//    server.run();
//
//    return 0;
//}

#include "Server.hpp"

//int	main(int ac, char **av)
int main() {

	// TODO : check argc
	try {
		//TODO : handle av[1] or default conf
		Server	webserv;
		
		webserv.connect();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}
