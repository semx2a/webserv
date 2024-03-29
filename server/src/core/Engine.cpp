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


void	Engine::connexionLoop() {

	struct epoll_event	event;
	int					nb_events;
	int					socket;

	while (true) {

		nb_events = this->_epoll.waitForConnexions();
		for (int i = 0; i < nb_events; ++i) {

			event = this->_epoll.readyEvent(i);
			socket = event.data.fd;
			if ((event.events & EPOLLERR) or (event.events & EPOLLHUP)) {
				#ifdef LOGS
				utl::log(socket, "Epoll error");
				#endif
				_endConnexion(socket);
			}
			else if (event.events & EPOLLRDHUP) {
				#ifdef LOGS
				utl::log(socket, "Closed connexion");
				#endif
				_endConnexion(socket);
			}
			else if (this->_epoll.isNewClient(socket)) {
				_addNewClient(socket);
			}
			else if (event.events & EPOLLIN) {
				_readFromClient(socket);
			}
			else if (event.events & EPOLLOUT) {
				_writeToClient(socket);
			}
		}
	}
}

void	Engine::_addNewClient(int serverSocket) {

	int clientSocket = accept(serverSocket, NULL, NULL);
	if (clientSocket < 0) {
		#ifdef LOGS
		utl::log(clientSocket, "Error: " + (std::string)strerror(errno));
		#endif
		return;
	}
	_epoll.addSocketToEpoll(clientSocket);
	_serverContexts[clientSocket] = _epoll.servers().find(serverSocket)->second;
	_buffers[clientSocket].setMaxBodySize(_serverContexts[clientSocket].maxBodySize());
	_status[clientSocket].setStatusCode("");
	#ifdef LOGS
	utl::log(clientSocket, "New client added");
	#endif
	
	int on = 1;
	setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(int));
}


void	Engine::_readFromClient(int clientSocket) {

	std::vector<char>	buffer(BUFFER_SIZE, '\0');

	try {
		int	bytesRead = recv(clientSocket, &buffer[0], buffer.size(), 0);
		if (bytesRead < 0) {
			throw HttpStatus("500");
		}
		else if (bytesRead == 0) { 
			#ifdef LOGS
			utl::log(clientSocket, "End of connexion");
			#endif
			_endConnexion(clientSocket);
			return ;
		}
		else {
			buffer.resize(bytesRead);
		}
		this->_buffers[clientSocket].add(buffer);
		this->_buffers[clientSocket].checkEnd();
		if (this->_buffers[clientSocket].contentLength() != 0)
			this->_buffers[clientSocket].setRemainingContentLength(this->_buffers[clientSocket].remainingContentLength() - bytesRead);
		_handleBuffer(clientSocket);
	}
	catch (HttpStatus& e) {
		this->_status[clientSocket].setStatusCode(e.statusCode());
		this->_epoll.editSocketInEpoll(clientSocket, EPOLLOUT);
	}
}

void	Engine::_handleBuffer(int clientSocket) {

	#ifdef LOGS
	utl::log(clientSocket, "Buffer received");
	#endif

	#ifdef DEBUG_ENGINE
	std::cout << RED << std::string(_buffers[clientSocket].raw().begin(), _buffers[clientSocket].raw().end()) << RESET << std::endl;
	#endif

	try {
		if (not this->_buffers[clientSocket].isEnded()) {
			#ifdef DEBUG_ENGINE
			std::cout << "[DEBUG] Request not ended" << std::endl;
			#endif
			return ;
		}
		if (this->_buffers[clientSocket].isEnded()	|| this->_status[clientSocket].statusCode() != "202"
														|| this->_status[clientSocket].statusCode() != "200") {
			this->_requests[clientSocket].parser(this->_buffers[clientSocket].raw(), this->_buffers[clientSocket].boundary());
			#ifdef LOGS
			utl::log(clientSocket, "Request parsed");
			#endif
			#ifdef DEBUG_ENGINE
			std::cout << RED << utl::vectorOfCharToStr(this->_buffers[clientSocket].raw()) << RESET << std::endl;
			#endif
			this->_epoll.editSocketInEpoll(clientSocket, EPOLLOUT);
		}
	}
	catch (HttpStatus& e) {
		this->_requests[clientSocket].parser(this->_buffers[clientSocket].raw(), this->_buffers[clientSocket].boundary());
		#ifdef LOGS
		utl::log(clientSocket, "Error in request");
		#endif
		#ifdef DEBUG_ENGINE
		std::cout << RED << utl::vectorOfCharToStr(this->_buffers[clientSocket].raw()) << RESET << std::endl;
		#endif
		this->_epoll.editSocketInEpoll(clientSocket, EPOLLOUT);
	}
	catch (std::exception& e) {
		#ifdef LOGS
		utl::log(clientSocket, "Error: " + (std::string)e.what() + ". Connexion closed.");
		#endif
		_endConnexion(clientSocket);
	}
}

void	Engine::_writeToClient(int clientSocket) {

	ResponseContext rc(this->_requests[clientSocket], this->_serverContexts[clientSocket]);
	Response res(this->_requests[clientSocket], rc, this->_status[clientSocket]);
	
	#ifdef LOGS
	utl::log(clientSocket, "Response about to be sent");
	#endif
	#ifdef DEBUG_ENGINE
	std::cout << GREEN << res.responseStr() << RESET << std::endl;
	#endif

	if ((send(clientSocket, res.responseStr().c_str(), res.responseStr().length(), 0)) < 0) {
		throw HttpStatus("500");
	}
	std::map<std::string, std::string>::const_iterator it = this->_requests[clientSocket].headers().find("Connexion");
	if (it != this->_requests[clientSocket].headers().end() && it->second == "close") {
		_endConnexion(clientSocket);
	}
	else {
		try {
			_buffers[clientSocket].clear();
			_epoll.editSocketInEpoll(clientSocket, EPOLLIN);
		}
		catch (std::exception& e) {
			#ifdef LOGS
			utl::log(clientSocket, "Error: " + (std::string)e.what() + ". Connexion closed.");
			#endif
			_endConnexion(clientSocket);
		}
	}
	this->_status[clientSocket].setStatusCode("");
}

void	Engine::_endConnexion(int clientSocket) {

	this->_buffers.erase(clientSocket);
	close(clientSocket);
}

