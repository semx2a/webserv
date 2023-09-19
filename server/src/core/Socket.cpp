#include "Socket.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Socket::Socket(int port) : _port(port) {

	this->_createSocket();
	this->setReusable();
	this->_setServerAddr();
	this->_bindSock();
	this->_startListening();
}


Socket::Socket(Socket const& rhs) : _port(rhs.getPort()) {

	*this = rhs;
}


Socket::~Socket() {}


Socket& Socket::operator=(Socket const& rhs) {

	if (this != &rhs) {
        // TODO
	}
	return *this;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

int		Socket::getPort() const { return this->_port; }

int		Socket::getFd() const { return this->_fd; }


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MEMBER FUNCTIONS

void	Socket::_createSocket() {

	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd == -1)
		throw std::runtime_error("socket(): " +(std::string) strerror(errno));
}


void	Socket::setReusable() {

	int on = 1;
	setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(int));
}


void	Socket::_setServerAddr() {

	this->_serverAddr.sin_family = AF_INET;
	this->_serverAddr.sin_port = htons(_port);
	this->_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
}


void	Socket::_bindSock() {

	if (bind(this->_fd,(struct sockaddr*)&this->_serverAddr, sizeof(this->_serverAddr)) == -1) {
		
		close(this->_fd);
		// TODO : close general des sockets
		throw std::runtime_error("bind(): " +(std::string) strerror(errno));
	}
}


void	Socket::_startListening() {

	if (listen(this->_fd, 5) == -1) {

		close(this->_fd);
		throw std::runtime_error("listen(): " +(std::string) strerror(errno));
	}
}

