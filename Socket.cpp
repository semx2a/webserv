#include "Socket.hpp"

/**********************************************************************************************************************/
/*										CONSTRUCTORS / DESTRUCTORS													  */
/**********************************************************************************************************************/
Socket::Socket (int port) : _port (port) {}

Socket::Socket (Socket const& rhs) : _port (rhs.getPort ()) {
	*this = rhs;
}

Socket::~Socket () {}

Socket& Socket::operator= (Socket const& rhs) {
	if (this != &rhs) {
        // TODO
	}
	return *this;
}

/**********************************************************************************************************************/
/*											GETTERS / SETTERS														  */
/**********************************************************************************************************************/
int		Socket::getPort () const { return _port; }

/**********************************************************************************************************************/
/*											MEMBER FUNCTIONS														  */
/**********************************************************************************************************************/
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

