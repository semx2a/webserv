#include "Engine.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Engine::Engine() {}

Engine::Engine(std::vector<ServerContext> const& serversContexts) : _epoll(serversContexts) {

	std::cout	<< BCYAN << "\n~~~Server ready~~~\n" << RESET << std::endl;
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
					utl::log(event_fd, "Epoll error");
					_endConnexion(event_fd);
				}
				else if (event.events & EPOLLRDHUP) {
					utl::log(event_fd, "Closed connexion");
					_endConnexion(event_fd);
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
	_status[clientSocket].setStatusCode("");
	utl::log(clientSocket, "New client added");
	
	int on = 1;
	setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(int));
}


void	Engine::_readFromClient(int clientFd) {

//	size_t buffersize = (this->_buffers[clientFd].hasBody() 
//						and this->_buffers[clientFd].remainingContentLength() < BUFFER_SIZE) 
//						? this->_buffers[clientFd].remainingContentLength() : BUFFER_SIZE;
//
	size_t buffersize = BUFFER_SIZE;
	std::vector<char>	buffer(buffersize, '\0');

	int	bytesRead = recv(clientFd, &buffer[0], buffer.size(), 0);
	if (bytesRead < 0) {
				std::cout << "ERROR: " << strerror(errno) << std::endl;

		throw HttpStatus("500");
	}
	else if (bytesRead == 0) { 
		utl::log(clientFd, "End of connexion");
		_endConnexion(clientFd);
	}
	else {
		buffer.resize(bytesRead);
	}
	try {
		this->_buffers[clientFd].add(buffer);
		this->_buffers[clientFd].checkEnd();
		this->_buffers[clientFd].setRemainingContentLength(this->_buffers[clientFd].remainingContentLength() - bytesRead);
		_handleBuffer(clientFd);
	}
	catch (HttpStatus& e) {
		this->_status[clientFd].setStatusCode(e.statusCode());
	}
}

void	Engine::_handleBuffer(int clientFd) {

	utl::log(clientFd, "Buffer received");
	std::cout << RED << std::string(_buffers[clientFd].raw().begin(), _buffers[clientFd].raw().end()) << RESET << std::endl;

	if (not this->_buffers[clientFd].isRequestEnded()) {
		std::cout << "Request not ended" << std::endl;
		return ;
	}
	
	this->_requests[clientFd].parser(this->_buffers[clientFd].raw());
	//this->_status[clientFd].setStatusCode("");
	if (this->_buffers[clientFd].isRequestEnded()	|| this->_status[clientFd].statusCode() != "202"
													|| this->_status[clientFd].statusCode() != "200") {
		this->_epoll.editSocketInEpoll(clientFd, EPOLLOUT);
	}
}

void	Engine::_writeToClient(int clientFd) {

	ResponseContext rc(this->_requests[clientFd], this->_serverContexts[clientFd]);
	Response res(this->_requests[clientFd], rc, this->_status[clientFd]);
	
	utl::log(clientFd, "Response about to be sent!");
	std::cout << GREEN << res.responseStr() << RESET << std::endl;

	if ((send(clientFd, res.responseStr().c_str(), res.responseStr().length(), 0)) < 0) {
		throw HttpStatus("500");
	}
	if (this->_requests[clientFd].header("Connection") == "close") {
		_endConnexion(clientFd);
	}
	else {
		//#ifdef DEBUG_ENGINE
		//std::cout << "[DEBUG] Keep-alive" << std::endl;
		//#endif
		_buffers[clientFd].clear();
		_epoll.editSocketInEpoll(clientFd, EPOLLIN);
	}
}

void	Engine::_endConnexion(int fd) {

	this->_buffers.erase(fd);
	close(fd);
}

