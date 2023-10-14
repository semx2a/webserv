#include "Epoll.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Epoll::Epoll() {}

Epoll::Epoll(std::vector<ServerContext> const& serversContexts) {

	_createEpollEvent();
	for (std::vector<ServerContext>::const_iterator serversIt = serversContexts.begin(); serversIt != serversContexts.end(); serversIt++) {

		for (std::map<std::string, int>::const_iterator ipPortIt = serversIt->listen().begin(); ipPortIt != serversIt->listen().end(); ipPortIt++) {

			try {
				int newServerListener = _pollPort(ipPortIt->first, ipPortIt->second);
				this->_servers[newServerListener] = *serversIt;
			}
			catch (const std::exception& e) {
				std::cerr << BOLD << "Error: " << RESET << e.what() << std::endl;
			}
		}
	}
	if (this->_servers.empty()) {
		throw std::runtime_error("No server to listen to");
	}
}


Epoll::Epoll(Epoll const& rhs) {

	*this = rhs;
}

Epoll& Epoll::operator=(Epoll const& rhs) {

	if (this != &rhs) {
		
		this->_servers = rhs.servers();
		this->_listener = rhs.listener();
		this->_toPoll = rhs.toPoll();
		for (int i = 0; i < MAX_EVENTS; i++) {
			this->_events[i] = rhs.readyEvent(i);
		}
	}
	return *this;
}

Epoll::~Epoll() {

	for (std::map<int, ServerContext>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++) {
		close(it->first);
	}
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

std::map<int, ServerContext> const&	Epoll::servers() const { return this->_servers; }
int									Epoll::listener() const { return this->_listener; }
struct epoll_event const&			Epoll::readyEvent(int index) const {	return this->_events[index]; }
struct epoll_event const&			Epoll::toPoll() const { return this->_toPoll; }

void	Epoll::setServers(std::map<int, ServerContext> const& servers) { this->_servers = servers; }
void	Epoll::setListener(int listener) { this->_listener = listener; }
void	Epoll::setReadyEvent(int index, struct epoll_event const& event) { this->_events[index] = event; }
void    Epoll::setToPoll(struct epoll_event const& toPoll) { this->_toPoll = toPoll; }


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CREATION

void	Epoll::_createEpollEvent() {

	this->_listener = epoll_create1(EPOLL_CLOEXEC);
	if (this->_listener < 0) {
		throw std::runtime_error(strerror(errno));
	}
}

int		Epoll::_pollPort(std::string const& ip, int port) {

	Socket newSocket(ip, port);
	int socket = newSocket.fd();
	addSocketToEpoll(socket);

	return socket;
}

void	Epoll::addSocketToEpoll(int socket) {

	std::memset((char *)&this->_toPoll, 0, sizeof(this->_toPoll));
	this->_toPoll.events = EPOLLIN; 
	this->_toPoll.data.fd = socket;  
	if (epoll_ctl(this->_listener, EPOLL_CTL_ADD, socket, &this->_toPoll) < 0) {
		throw std::runtime_error(strerror(errno));
	}
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MODIFICATION

void	Epoll::editSocketInEpoll(int socket, int eventToWatch) {

	std::memset((char *)&this->_toPoll, 0, sizeof(this->_toPoll));
	this->_toPoll.events = eventToWatch; 
	this->_toPoll.data.fd = socket;  
	if (epoll_ctl(this->_listener, EPOLL_CTL_MOD, socket, &this->_toPoll) < 0) {
		throw std::runtime_error(strerror(errno));
	}
}

int		Epoll::waitForConnexions() {

	#ifdef LOGS
	std::cout << utl::print_wait() << std::endl;
	#endif
	int numEvents = epoll_wait(this->_listener, this->_events, MAX_EVENTS, -1);
	if (numEvents < 0) {
		throw std::runtime_error(strerror(errno));
	}
	return numEvents;
}

bool	Epoll::isNewClient(int socket) const {

	return (this->_servers.find(socket) != this->_servers.end());
}
