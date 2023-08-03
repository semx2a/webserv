#include "Socket.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Socket::Socket (int port) : port (port) {

	createSocket ();
	setReusable ();
	setServerAddr ();
	bindSock ();
	startListening ();
}


Socket::Socket (Socket const& rhs) : port (rhs.getPort ()) {

	*this = rhs;
}


Socket::~Socket () {}


Socket& Socket::operator= (Socket const& rhs) {

	if (this != &rhs) {
        // TODO
	}
	return *this;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

int		Socket::getPort () const { return port; }

int		Socket::getFd () const { return fd; }


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MEMBER FUNCTIONS

void	Socket::createSocket () {

	fd = socket (AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
		throw std::runtime_error ("socket (): " + (std::string) strerror (errno));
}


void	Socket::setReusable () {

	int on = 1;
	setsockopt (fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof (int));
}


void	Socket::setServerAddr () {

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons (port);
	serverAddr.sin_addr.s_addr = htonl (INADDR_ANY);
}


void	Socket::bindSock () {

	if (bind (fd, (struct sockaddr*)&serverAddr, sizeof (serverAddr)) == -1) {
		close (fd);
		// TODO : close general des sockets
		throw std::runtime_error ("bind (): " + (std::string) strerror (errno));
	}
}


void	Socket::startListening () {

	if (listen (fd, 5) == -1) {
		close (fd);
		throw std::runtime_error ("listen (): " + (std::string) strerror (errno));
	}
}

