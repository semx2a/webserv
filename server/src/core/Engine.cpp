#include "Engine.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Engine::Engine() {}

Engine::Engine(std::vector<ServerContext> const& serversContexts) : _epoll(serversContexts) {

	std::cout	<< BCYAN << "\n~~~Server ready~~~\n" << NO_COLOR << std::endl;
}

Engine::Engine(Engine const& rhs) : _epoll(rhs.epoll()) {
	*this = rhs;
}

Engine::~Engine() {}


Engine& Engine::operator=(Engine const& rhs) {

	if (this != &rhs) {
        
		this->_epoll = rhs.epoll();
		this->_buffers = rhs.buffers();
		this->_requests = rhs.requests();
		this->_serverContexts = rhs.serverContexts();
	}
	return *this;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

Epoll const&						Engine::epoll() const { return this->_epoll; }
std::map<int, ServerContext> const&	Engine::serverContexts() const { return this->_serverContexts; }
std::map<int, Buffer> const&		Engine::buffers() const { return this->_buffers; }
std::map<int, Request> const&		Engine::requests() const { return this->_requests; }

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

				event = this->_epoll.readyEvent(i);
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
	_serverContexts[clientSocket] = _epoll.servers().find(serverFd)->second;
	_buffers[clientSocket].setMaxBodySize(_serverContexts[clientSocket].maxBodySize());
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

	log(clientFd, "Buffer received");
	std::cout << RED << &this->_buffers[clientFd].raw()[0] << NO_COLOR << std::endl;

	if (!this->_buffers[clientFd].isRequestEnded())
		return ;

	this->_requests[clientFd].parser(this->_buffers[clientFd].raw());
	this->_epoll.editSocketInEpoll(clientFd, EPOLLOUT);
}

void	Engine::_writeToClient(int clientFd) {

	Response res(this->_requests[clientFd], this->_serverContexts[clientFd]);
	if (res.request().method() == "GET") {
		res.handleGet();
	}
	log(clientFd, "Response about to be sent!");
	std::cout << RED << res.responseStr() << NO_COLOR << std::endl;

	if ((send(clientFd, res.responseStr().c_str(), res.responseStr().length(), 0)) < 0) {
		throw std::runtime_error(SENDERR);
	}
	//TODO: dont close if header keep-alive
	if (this->_requests[clientFd].header("Connection") == "close")
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

