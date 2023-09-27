#include "Epoll.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Epoll::Epoll() {}

Epoll::Epoll(std::vector<ServerContext> const& serversContexts) {

	try {
		_createEpollEvent();

		for (std::vector<ServerContext>::const_iterator serversIt = serversContexts.begin(); serversIt != serversContexts.end(); serversIt++) {

			for (std::map<std::string, int>::const_iterator ipPortIt = serversIt->getListen().begin(); ipPortIt != serversIt->getListen().end(); ipPortIt++) {

				#ifdef DEBUG
				std::cout << "Listening on " << ipPortIt->first << ": " << ipPortIt->second << std::endl;
				#endif
				int newServerListener = _pollPort(ipPortIt->first, ipPortIt->second);
				this->_servers[newServerListener] = *serversIt;
			}
		}
	}
	catch(const std::exception& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;	
	}
}


Epoll::Epoll(Epoll const& rhs) {

	*this = rhs;
}

Epoll& Epoll::operator=(Epoll const& rhs) {

	if (this != &rhs) {
        // TODO
	}
	return *this;
}

Epoll::~Epoll() {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

struct epoll_event const&	Epoll::getReadyEvent(int index) const {

	return this->_events[index];
}

std::map<int, ServerContext> const&	Epoll::getServers() const { 

	return this->_servers;
}

bool	Epoll::isNewClient(int fd) {

	return this->_servers.find(fd) != this->_servers.end();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CREATION

void	Epoll::_createEpollEvent() {

	this->_listener = epoll_create(10); // TODO : fix number
	if (this->_listener < 0) {
		throw std::runtime_error(ECREATERR);
	}
}

int		Epoll::_pollPort(std::string const& ip, int port) {

	Socket newSocket(ip, port);
	int fd = newSocket.getFd();
	addSocketToEpoll(fd);

	return fd;
}

void	Epoll::addSocketToEpoll(int fd) {

	std::memset((char *)&this->_toPoll, 0, sizeof(this->_toPoll));
	this->_toPoll.events = EPOLLIN; 
	this->_toPoll.data.fd = fd;  
	if (epoll_ctl(this->_listener, EPOLL_CTL_ADD, fd, &this->_toPoll) < 0) {
		throw std::runtime_error(ECTLERR);
	}
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MODIFICATION

void	Epoll::editSocketInEpoll(int fd, int eventToWatch) {

	std::memset((char *)&this->_toPoll, 0, sizeof(this->_toPoll));
	this->_toPoll.events = eventToWatch; 
	this->_toPoll.data.fd = fd;  
	if (epoll_ctl(this->_listener, EPOLL_CTL_MOD, fd, &this->_toPoll) < 0) {
		throw std::runtime_error(ECTLERR);
	}
}

int		Epoll::waitForConnexions() {

	int numEvents = epoll_wait(this->_listener, this->_events, MAX_EVENTS, -1);
	if (numEvents < 0) {
		throw std::runtime_error(EWAITERR);
	}
	print_wait();
	return numEvents;
}
