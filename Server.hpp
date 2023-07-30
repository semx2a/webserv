#include <cerrno>
#include <exception>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "Epoll.hpp"

const int BUFFER_SIZE = 1024;

class Server {

	public:
		Server ();
		Server (std::string conf_file);
		Server (Server const& rhs);
		~Server ();
		Server& operator= (Server const& rhs);

	private:
		int fd;
		struct sockaddr_in address;
		Epoll events (int socketFd);


};
