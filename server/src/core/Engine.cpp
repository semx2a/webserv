#include "Engine.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

//Engine::Engine () {}

Engine::Engine(std::vector<ServerContext> const& serversContexts) :	_serversContexts(serversContexts), 
																	_epollEvents(serversContexts) {
}

Engine::Engine(Engine const& rhs) : _serversContexts(rhs._serversContexts), 
									_epollEvents(rhs._epollEvents) {
	*this = rhs;
}

Engine::~Engine() {

	// TODO
}


Engine& Engine::operator=(Engine const& rhs) {

	if (this != &rhs) {
        // TODO
	}
	return *this;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

std::vector<ServerContext> const&	Engine::getServerContexts() const { return this->_serversContexts; }

Epoll const&						Engine::getEpollEvents() const { return this->_epollEvents; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MEMBER FUNCTIONS


void	Engine::connect() {

	struct epoll_event	event;
	int					nb_events;
	int					event_fd;

	try {

		while (true) {

			nb_events = this->_epollEvents.waitForConnexions();
			for (int i = 0; i < nb_events; ++i) {

				event = this->_epollEvents.getReadyEvent(i);
				event_fd = event.data.fd;
				if ((event.events & EPOLLERR) ||(event.events & EPOLLHUP)) {
					log(event_fd, "Epoll error");
					_closeSocket(event_fd);
				}
				else if (event.events & EPOLLRDHUP) {
					log(event_fd, "Closed connexion");
					_closeSocket(event_fd);
				}
				else if (this->_epollEvents.isNewClient(event_fd)) {
					_addNewClient(event_fd);
				}
				else if (event.events & EPOLLIN) {
					_readFromClient(event_fd);
				}
				else if (event.events & EPOLLOUT) {
					_writeToClient(event_fd);
				}
			}
		}
	}
	catch (std::exception& e) {

		std::cerr << "ERROR: " << e.what() << std::endl;
	}
}

void	Engine::_addNewClient(int serverFd) {

	int	clientSocket;

	try {
		clientSocket = accept(serverFd, NULL, NULL);
		if (clientSocket < 0) {
			throw std::runtime_error(ACCEPTERR);
		}
	}
	catch(const std::exception& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	_epollEvents.addSocketToEpoll(clientSocket);
	_clientDataMap[clientSocket].setServerContext(_epollEvents.getServers().find(serverFd)->second);
	log(clientSocket, "New request");
	// TODO : set reusable ?	
}


void	Engine::_readFromClient(int clientFd) {

	std::vector<char>	buffer(BUFFER_SIZE, '\0');

	int	bytesRead = recv(clientFd, &buffer[0], buffer.size(), 0);
	if (bytesRead < 0) {
		throw std::runtime_error(RECVERR);
	}
	else if (bytesRead == 0) { 
		log(clientFd, "End of connexion");
		_closeSocket(clientFd);
	}
	else {
		buffer.resize(bytesRead);
	}
	this->_clientDataMap[clientFd].addToBuffer(buffer);
	_handleClientData(clientFd);
}

void	Engine::_writeToClient(int clientFd) {

	Response	res;
	
	res.buildResponse(this->_clientRequest, this->_serverContexts[0], clientFd);
	
	if ((send(clientFd, res.getResponse().c_str(), res.getResponse().length(), 0)) < 0) {
		throw std::runtime_error(SENDERR);
	}
	//TODO: dont close if header keep-alive
	if (this->_clientRequest.getHeader("Connection") == "close")
		_closeSocket(clientFd);
}


void	Engine::_handleClientData(int clientFd) {

	if (!this->_clientDataMap[clientFd].isRequestEnded())
		return ;

	#ifdef DEBUG
	std::cout << &this->_clientDataMap[clientFd].getRequest()[0] << std::endl;
	#endif
	this->_clientRequest.parser(_clientDataMap[clientFd].getRequest());
	this->_epollEvents.editSocketInEpoll(clientFd, EPOLLOUT);
}


void	Engine::_closeSocket(int fd) {

	this->_clientDataMap.erase(fd);
	close(fd);
}
