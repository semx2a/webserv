#include "Server.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Server::Server() :	_config(), 
					_epollEvents(this->_config.getPorts()) {
}

Server::Server(std::string const& conf_file) :	_config(conf_file), 
												_epollEvents(this->_config.getPorts()) {
}

Server::Server(Server const& rhs) {

	*this = rhs;
}

Server::~Server() {

	// TODO
}


Server& Server::operator=(Server const& rhs) {

	if (this != &rhs) {
        // TODO
	}
	return *this;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

Config const&	Server::getConfig() const { return this->_config; }

Epoll const&	Server::getEpollEvents() const { return this->_epollEvents; }


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MEMBER FUNCTIONS


void	Server::connect() {

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
					this->_epollEvents.addNewClient(event_fd);
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

void	Server::_readFromClient(int clientFd) {

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
	//this->_clientDataMap[clientFd].insert(this->_clientDataMap[clientFd].end(), buffer.begin(), buffer.end());
	this->_clientDataMap[clientFd].addToBuffer(buffer);
	_handleClientData(clientFd);
}

void	Server::_writeToClient(int clientFd) {

	//TODO: Response	response(this->_clientRequest, this->_config, clientFd);
	
	std::string message = "Request received";
	if ((send(clientFd, message.c_str(), message.length(), 0)) < 0) {
		throw std::runtime_error(SENDERR);
	}
	//TODO: dont close if header keep-alive
	_closeSocket(clientFd);
}


void	Server::_handleClientData(int clientFd) {

	if (!this->_clientDataMap[clientFd].isRequestEnded())
		return ;

	#ifdef DEBUG
	std::cout << &this->_clientDataMap[clientFd].getRequest()[0] << std::endl;
	#endif
	this->_clientRequest.parser(_clientDataMap[clientFd].getRequest());
	this->_epollEvents.editSocketInEpoll(clientFd, EPOLLOUT);
}


void	Server::_closeSocket(int fd) {

	this->_clientDataMap.erase(fd);
	close(fd);
}
