#include "Socket.hpp"

//:::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Socket::Socket() {}

Socket::Socket(std::string const& ip, int port) : _ip(ip), _port(port) {

	this->_createSocket();
	this->setReusable();
	this->_setServerAddr();
	this->_bindSock();
	this->_startListening();
}

Socket::Socket(Socket const& rhs) : _port(rhs.port()) {

	*this = rhs;
}

Socket::~Socket() {}

Socket& Socket::operator=(Socket const& rhs) {

	if (this != &rhs) {
		
		this->_fd = rhs.fd();
		this->_ip = rhs.ip();
		this->_port = rhs.port();
		this->_serverAddr = rhs.serverAddr();
	}
	return *this;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

int							Socket::fd() const { return this->_fd; }
std::string	const &			Socket::ip() const { return this->_ip; }
int							Socket::port() const { return this->_port; }
struct sockaddr_in const &	Socket::serverAddr() const { return this->_serverAddr; }

void	Socket::setFd(int fd) { this->_fd = fd; }
void	Socket::setIp(std::string const& ip) { this->_ip = ip; }
void	Socket::setPort(int port) { this->_port = port; }
void	Socket::setServerAddr(struct sockaddr_in const& serverAddr) { this->_serverAddr = serverAddr; }


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MEMBER FUNCTIONS

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
	this->_serverAddr.sin_addr.s_addr = htonl(_intIp(_ip));
}

void	Socket::_bindSock() {

	std::cout << RED << "Binding listening socket " << this->_fd << " to port " << this->_port << " ..." << NO_COLOR << std::endl;
	if (bind(this->_fd, (struct sockaddr*)&this->_serverAddr, sizeof(this->_serverAddr)) == -1) {
		
		close(this->_fd);
		// TODO : close general des sockets
		std::stringstream err;
		err << "bind(): " << this->_port << " " << strerror(errno);
		throw std::runtime_error(err.str());
	}
}


void	Socket::_startListening() {

	if (listen(this->_fd, 5) == -1) {

		close(this->_fd);
		throw std::runtime_error("listen(): " +(std::string) strerror(errno));
	}
}


uint32_t	Socket::_intIp(std::string const& ip) {

	uint32_t result = 0;
	std::istringstream ss(ip);
	std::string token;

	for(int i = 0; i < 4; i++) {
		std::getline(ss, token, '.');
		result = (result << 8) | (static_cast<uint32_t>(std::atoi(token.c_str())) & 0xFF);
	}

	return result;
}