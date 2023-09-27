# include <cstring>
#include <fstream>
# include <iostream>
# include <netinet/in.h>
# include <stdexcept>
#include <string>
# include <sys/socket.h>
# include <unistd.h>

int main (int ac, char** av)
{
	try {

		if (ac != 2)
			throw std::invalid_argument ("Format: ./Client <request_file>");
		std::ifstream request_file(av[1]);
    
		if (!request_file.is_open()) 
			throw std::runtime_error ("File could not be opened");

		std::string	request((std::istreambuf_iterator<char>(request_file)), std::istreambuf_iterator<char>());
		request_file.close();

		std::cout << request << std::endl; 

		struct sockaddr_in	_server_addr;
		socklen_t			_server_addrlen = sizeof (_server_addr);
		int					domain = AF_INET;
		int					service = SOCK_STREAM;
		int					protocol = 0;
		int					port = 8080;
		int					interface = INADDR_ANY;

		memset (&_server_addr, 0, _server_addrlen);
		_server_addr.sin_family = domain;
		_server_addr.sin_port = htons (port); // set the bits to be in order for the network
		_server_addr.sin_addr.s_addr = htonl (interface);
		_server_addrlen = sizeof (_server_addr);

		int sockfd = socket (domain, service, protocol);
		if (sockfd < 0)
			throw std::runtime_error ("Socket creation failed");

		if (connect (sockfd, (struct sockaddr *)&_server_addr, _server_addrlen) < 0)
			throw std::runtime_error ("Client socket connect () failed");

		//std::string request1 = "yo";
		//send (sockfd, request1.c_str (), request1.size () , 0 );
		send (sockfd, request.c_str (), request.size () , 0 );
		std::cout << "Request sent" << std::endl;
		char buffer[1024] = {0};
		recv (sockfd, buffer, 1024, 0);
		std::cout << buffer << std::endl;
	}
	catch (const std::exception& e) {
		
		std::cerr << "Error: " << e.what () << std::endl;
	}
}
