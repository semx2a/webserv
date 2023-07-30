#include "Epoll.hpp"

Epoll::Epoll () {

	// TODO
}

Epoll::Epoll (std::vector <int> ports) {
	try {
		for (std::vector <int>::iterator it = ports.begin (); it != ports.end (); it++) {
			_sockFds.push_back (pollPort (*it));
		}
	} catch (const std::exception& e) {
		std::cerr << "Epoll: " << e.what () << std::endl;	
	}
}

Epoll::Epoll (Epoll const& rhs) {
	*this = rhs;
}

Epoll::~Epoll () {}

Epoll& Epoll::operator= (Epoll const& rhs) {
	if (this != &rhs) {

	}
	return *this;
}


/*
**	Connexion
*/

void	Epoll::createEpollEvent () {
	_epollFd = epoll_create (10); // TODO : fix number
	if (_epollFd == -1) {
		close (_sockFd);
		throw std::runtime_error ("epoll_create1 (): " + (std::string) strerror (errno));
	}

}

int		Epoll::pollPort (int port) {

	Socket newSocket (port);
	newSocket.createSocket ();
	newSocket.setReusable ();
	newSocket.setServerAddr ();
	newSocket.bindSock ();
	newSocket.startListening ();
	addSocketToEpoll (newSocket.getFd ());



	struct epoll_event events [MAX_EVENTS];

	int	clientSocket;
	struct sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof (clientAddress);

	return _sockFd;
}

void	Socket::createSocket () {
	_sockFd = socket (AF_INET, SOCK_STREAM, 0);
	if (_sockFd == -1)
		throw std::runtime_error ("socket (): " + (std::string) strerror (errno));
}

void	Socket::setReusable () {
	int on = 1;
	setsockopt (_sockFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof (int));

}

void	Socket::setServerAddr () {
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons (_port);
	_serverAddr.sin_addr.s_addr = htonl (INADDR_ANY);
}

void	Socket::bindSock () {
	if (bind (_sockFd, (struct sockaddr*)&_serverAddr, sizeof (_serverAddr)) == -1) {
		close (_sockFd);
		// TODO : close general des sockets
		throw std::runtime_error ("bind (): " + (std::string) strerror (errno));
	}
}

void	Socket::startListening () {
	if (listen (_sockFd, 5) == -1) {
		close (_sockFd);
		throw std::runtime_error ("listen (): " + (std::string) strerror (errno));
	}
}

void	Epoll::addSocketToEpoll (int fd) {
	struct epoll_event	to_poll;

	std::memset ((char *)&to_poll, 0, sizeof (to_poll));

	to_poll.events = EPOLLIN; 
	to_poll.data.fd = fd;  

	if (epoll_ctl (_epollFd, EPOLL_CTL_ADD, fd, &to_poll) == -1)
	{
		close (fd);
		close (_epollFd);
		throw std::runtime_error ("epoll_ctl (): " + (std::string) strerror (errno));
	}
}

void	Epoll::editSocketInEpoll () {}

