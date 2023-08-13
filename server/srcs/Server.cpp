#include "../inc/Server.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Server::Server () :	config (), 
					epollEvents (config.getPorts ()) {
}

Server::Server (std::string const& conf_file) :	config (conf_file), 
												epollEvents (config.getPorts ()) {
}

Server::Server (Server const& rhs) {

	*this = rhs;
}

Server::~Server () {

	// TODO
}


Server& Server::operator= (Server const& rhs) {

	if (this != &rhs) {
        // TODO
	}
	return *this;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

Config const&	Server::getConfig () const { return config; }

Epoll const&	Server::getEpollEvents () const { return epollEvents; }


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MEMBER FUNCTIONS


void	Server::connect () {

	struct epoll_event	event;
	int					nb_events;
	int					clientFd;

	try {

		while (true) {

			nb_events = epollEvents.waitForConnexions ();
			for (int i = 0; i < nb_events; ++i) {

				event = epollEvents.getReadyEvent (i);
				clientFd = event.data.fd;
				if ((event.events & EPOLLERR) || (event.events & EPOLLHUP) || (event.events & EPOLLRDHUP)) {
					log (clientFd, "End of connexion");
					clientData.erase (clientFd);
					close (clientFd);
				}
				else if (epollEvents.isNewClient (clientFd)) {
					epollEvents.addNewClient (clientFd);
				}
				else if (event.events & EPOLLIN) {
					readFromClient (clientFd);
				}
				else if (event.events & EPOLLOUT) {
					writeToClient (clientFd);
				}
			}
		}
	}
	catch (std::exception& e) {

		std::cerr << "ERROR: " << e.what () << std::endl;
	}
}

void	Server::readFromClient (int clientFd) {

	std::vector <char>	buffer = epollEvents.receiveBuffer (clientFd);

	clientData [clientFd].insert (clientData [clientFd].end (), buffer.begin (), buffer.end ());
	if(isRequestEnded (clientFd)) {
		handleRequest (clientFd);
	}
}

void	Server::writeToClient (int clientFd) {

	std::string message = "Request received";
	if ((send (clientFd, message.c_str (), message.length (), 0)) < 0) {
		throw std::runtime_error (SENDERR);
	}
	clientData.erase (clientFd);
	close (clientFd);
}


void	Server::handleRequest (int clientFd) {

	std::string str;
	str.assign(&clientData[clientFd][0]);
	#ifdef DEBUG
	std::cout << &clientData[clientFd][0] << std::endl;
	#endif
	clientRequest.parser(str);
	epollEvents.editSocketInEpoll (clientFd, EPOLLOUT);
}

bool	Server::isRequestEnded (int clientFd) {

	std::string	end_of_data(&clientData[clientFd].end()[-4], &clientData[clientFd].end()[0]);

	if (clientData[clientFd].size() > 4 && end_of_data == "\r\n\r\n")
		return true;
	return false;
}

