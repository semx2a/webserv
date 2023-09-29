#include "Engine.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Engine::Engine() {}

Engine::Engine(std::vector<ServerContext> const& serversContexts) : _epoll(serversContexts) {

	std::cout	<< BCYAN << "\n~~~Server initialized~~~\n" << NO_COLOR << std::endl;
}

Engine::Engine(Engine const& rhs) : _epoll(rhs.getEpollEvents()) {
	*this = rhs;
}

Engine::~Engine() {}


Engine& Engine::operator=(Engine const& rhs) {

	if (this != &rhs) {
        
		this->_epoll = rhs.getEpollEvents();
		this->_buffers = rhs.getBuffers();
		this->_requests = rhs.getRequests();
		this->_serverContexts = rhs.getServersContexts();
	}
	return *this;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

Epoll const&						Engine::getEpollEvents() const { return this->_epoll; }
std::map<int, ServerContext> const&	Engine::getServersContexts() const { return this->_serverContexts; }
std::map<int, Buffer> const&		Engine::getBuffers() const { return this->_buffers; }
std::map<int, Request> const&		Engine::getRequests() const { return this->_requests; }

void	Engine::setEpollEvents(Epoll const& epollEvents) { this->_epoll = epollEvents; }
void	Engine::setServersContexts(std::map<int, ServerContext> const& serversContextsMap) { this->_serverContexts = serversContextsMap; }
void	Engine::setBuffers(std::map<int, Buffer> const& buffersMap) { this->_buffers = buffersMap; }
void	Engine::setRequests(std::map<int, Request> const& requestsMap) { this->_requests = requestsMap; }


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MEMBER FUNCTIONS


void	Engine::connect() {

	struct epoll_event	event;
	int					nb_events;
	int					event_fd;

	try {

		while (true) {

			nb_events = this->_epoll.waitForConnexions();
			for (int i = 0; i < nb_events; ++i) {

				event = this->_epoll.getReadyEvent(i);
				event_fd = event.data.fd;
				if ((event.events & EPOLLERR) ||(event.events & EPOLLHUP)) {
					log(event_fd, "Epoll error");
					_closeSocket(event_fd);
				}
				else if (event.events & EPOLLRDHUP) {
					log(event_fd, "Closed connexion");
					_closeSocket(event_fd);
				}
				else if (this->_epoll.isNewClient(event_fd)) {
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
	_epoll.addSocketToEpoll(clientSocket);
	_serverContexts[clientSocket] = _epoll.getServers().find(serverFd)->second;
	_buffers[clientSocket].setMaxBodySize(_serverContexts[clientSocket].getMaxBodySize());
	log(clientSocket, "New client added");
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
	this->_buffers[clientFd].add(buffer);
	// TODO: catch RequestError 
	// atoi du e.what()
	// case switch sur le code d'erreur
	this->_buffers[clientFd].checkEnd();
	_handleBuffer(clientFd);
}

void	Engine::_handleBuffer(int clientFd) {

	#ifdef DEBUG_BUFFER
		log(clientFd, "Buffer received");
		std::cout << RED << &this->_buffers[clientFd].getRaw()[0] << NO_COLOR << std::endl;
	#endif

	if (!this->_buffers[clientFd].isRequestEnded())
		return ;

	this->_requests[clientFd].parser(this->_buffers[clientFd].getRaw());
	this->_epoll.editSocketInEpoll(clientFd, EPOLLOUT);
}

void	Engine::_writeToClient(int clientFd) {

	Response		res(this->_requests[clientFd], this->_serverContexts[clientFd]);
	ResponseHandler	resHandler(&res);
	
	resHandler.handleResponse();
	res.buildResponse();
	#ifdef DEBUG_RESPONSE
		log(clientFd, "Response about to be sent!");
		std::cout << RED << res.getResponse() << NO_COLOR << std::endl;
	#endif
	if ((send(clientFd, res.getResponse().c_str(), res.getResponse().length(), 0)) < 0) {
		throw std::runtime_error(SENDERR);
	}
	//TODO: dont close if header keep-alive
	if (this->_requests[clientFd].getHeader("Connection") == "close")
		_closeSocket(clientFd);
	else
	{
		_buffers.erase(clientFd);
		_epoll.editSocketInEpoll(clientFd, EPOLLIN);
	}
}




void	Engine::_closeSocket(int fd) {

	this->_buffers.erase(fd);
	close(fd);
}

