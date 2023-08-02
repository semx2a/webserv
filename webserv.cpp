#include "Server.hpp"

//int	main (int ac, char **av)
int main () {

	// TODO : check argc
	try {
		//TODO : server (argv [1]);
		Server	WebServ;
		
		WebServ.connect ();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what () << std::endl;
	}
	return 0;
}
