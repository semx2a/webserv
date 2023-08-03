#include "Server.hpp"

//int	main (int ac, char **av)
int main () {

	// TODO : check argc
	try {
		//TODO : handle av [1] or default conf
		Config	server_conf ("");
		Server	webserv (server_conf);
		
		webserv.connect ();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what () << std::endl;
	}
	return 0;
}
