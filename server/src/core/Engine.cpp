#include "Engine.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Engine::Engine() {}

Engine::Engine(std::vector<ServerContext> const& serversContexts) :	_epollEvents(serversContexts) {
}

Engine::Engine(Engine const& rhs) : _epollEvents(rhs.getEpollEvents()) {
	*this = rhs;
}

Engine::~Engine() {}


Engine& Engine::operator=(Engine const& rhs) {

	if (this != &rhs) {
        
		this->_epollEvents = rhs.getEpollEvents();
		this->_buffersMap = rhs.getBuffersMap();
		this->_requestsMap = rhs.getRequestsMap();
		this->_serverContextsMap = rhs.getServersContexts();
	}
	return *this;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

Epoll const&						Engine::getEpollEvents() const { return this->_epollEvents; }
std::map<int, ServerContext> const&	Engine::getServersContexts() const { return this->_serverContextsMap; }
std::map<int, Buffer> const&		Engine::getBuffersMap() const { return this->_buffersMap; }
std::map<int, Request> const&		Engine::getRequestsMap() const { return this->_requestsMap; }

void	Engine::setEpollEvents(Epoll const& epollEvents) { this->_epollEvents = epollEvents; }
void	Engine::setServersContexts(std::map<int, ServerContext> const& serversContextsMap) { this->_serverContextsMap = serversContextsMap; }
void	Engine::setBuffersMap(std::map<int, Buffer> const& buffersMap) { this->_buffersMap = buffersMap; }
void	Engine::setRequestsMap(std::map<int, Request> const& requestsMap) { this->_requestsMap = requestsMap; }


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
	//_buffersMap[clientSocket].setServerContext(_epollEvents.getServers().find(serverFd)->second);
	_serverContextsMap[clientSocket] = _epollEvents.getServers().find(serverFd)->second;
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
	this->_buffersMap[clientFd].add(buffer);
	this->_buffersMap[clientFd].checkEnd();
	_handleBuffer(clientFd);
}

void	Engine::_handleBuffer(int clientFd) {

	if (!this->_buffersMap[clientFd].isRequestEnded())
		return ;

	#ifdef DEBUG
	std::cout << std::endl;
	std::cout << RED << "_________________________________________________________" << NO_COLOR << std::endl;
	std::cout << RED << "BUFFER of client " << clientFd << ":" << NO_COLOR << std::endl;
	std::cout << &this->_buffersMap[clientFd].getRaw()[0] << std::endl;
	std::cout << RED << "_________________________________________________________" << NO_COLOR << std::endl;
	#endif
	//this->_clientRequest.parser(_buffersMap[clientFd].getRaw());
	this->_requestsMap[clientFd].parser(this->_buffersMap[clientFd].getRaw());
	//this->_buffersMap[clientFd].getRequest().parser(_buffersMap[clientFd].getRaw());
	this->_epollEvents.editSocketInEpoll(clientFd, EPOLLOUT);
}

void	Engine::_writeToClient(int clientFd) {

	Response	res;
	
	res.buildResponse(this->_requestsMap[clientFd], this->_serverContextsMap[clientFd]);
	
	std::cout << RED << "Response: " << res.getResponse() << NO_COLOR << std::endl;
	if ((send(clientFd, res.getResponse().c_str(), res.getResponse().length(), 0)) < 0) {
		throw std::runtime_error(SENDERR);
	}
	//TODO: dont close if header keep-alive
	if (this->_requestsMap[clientFd].getHeader("Connection") == "close")
		_closeSocket(clientFd);
	else
	{
		_buffersMap.erase(clientFd);
		_epollEvents.editSocketInEpoll(clientFd, EPOLLIN);
	}
}




void	Engine::_closeSocket(int fd) {

	this->_buffersMap.erase(fd);
	close(fd);
}
