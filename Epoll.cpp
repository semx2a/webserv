#include <asm-generic/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>

#include "Epoll.hpp"

Epoll::Epoll () {

}

Epoll::Epoll (std::vector <int> ports) {

	try {
	
		for (std::vector <int>::iterator it = ports.begin (); it != ports.end (); it++) {
				
			sockFds.push_back (pollPort (*it));
		}
	}
	catch (const std::exception& e) {
		
	}
}

Epoll::Epoll (Epoll const& rhs) {

	*this = rhs;
}

Epoll::~Epoll () {}

Epoll& Epoll::operator= (Epoll const& rhs) {

	if (this != &rhs)
	{

	}
	return *this;
}


/*
**	Connexion
*/

int	Epoll::pollPort (int port) {

	int socketFd = createSocket ();
	setReusable (socketFd);
	struct sockaddr_in serverAddress;

	int on = 1;
	setsockopt (socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof (int));

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons (8080);  // Port 8080
	serverAddress.sin_addr.s_addr = htonl (INADDR_ANY);

	if (bind (socketFd, (struct sockaddr*)&serverAddress, sizeof (serverAddress)) == -1) 
	{
		close (socketFd);
		// TODO : close general des sockets
		throw std::runtime_error ("bind (): " + (std::string) strerror (errno));
	}
	if (listen (socketFd, 5) == -1)
	{
		close (socketFd);
		throw std::runtime_error ("listen (): " + (std::string) strerror (errno));
	}
	int epollFd = epoll_create1 (0);
	if (epollFd == -1)
	{
		close (socketFd);
		throw std::runtime_error ("epoll_create1 (): " + (std::string) strerror (errno));
	}

	struct epoll_event to_poll;
	to_poll.events = EPOLLIN; 
	to_poll.data.fd = socketFd;  

	if (epoll_ctl (epollFd, EPOLL_CTL_ADD, socketFd, &to_poll) == -1)
	{
		close (socketFd);
		close (epollFd);
		throw std::runtime_error ("epoll_ctl (): " + (std::string) strerror (errno));
	}

	struct epoll_event events [MAX_EVENTS];

	int	clientSocket;
	struct sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof (clientAddress);

	return socketFd;
}

int		Epoll::createSocket () {

	int socketFd;

	socketFd = socket (AF_INET, SOCK_STREAM, 0);
	if (socketFd == -1)
		throw std::runtime_error ("socket (): " + (std::string) strerror (errno));

	return socketFd;
}

void	Epoll::setReusable (int sockFd) {

}

void	setReusable (int sockFd) {

}

void	bindSock (int sockFd) {

}

void	startListening (int sockFd) {

}

void	createEpollEvent (int sockFd) {

}

