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

	try {

		while (true) {

			nb_events = epollEvents.waitForConnexions ();
			for (int i = 0; i < nb_events; ++i) {

				event = epollEvents.getReadyEvent (i);
				if ((event.events & EPOLLERR) || (event.events & EPOLLHUP) || (event.events & EPOLLRDHUP)) {
					log (event.data.fd, "End of connexion");
					close (event.data.fd);// TODO : delete corresponding nodes in client map
				}
				else if (epollEvents.isNewClient (event.data.fd)) {
					epollEvents.addNewClient (event.data.fd);
				}
				else if (event.events & EPOLLIN) {
					readFromClient (event.data.fd);
					//clientRequest.parser(epollEvents.readFromClient (event.data.fd));
				}
				else if (event.events & EPOLLOUT) {
					writeToClient (event.data.fd);
				}
			}
		}
	}
	catch (std::exception& e) {

		std::cerr << "ERROR: " << e.what () << std::endl;
	}
}


void	Server::readFromClient (int fd) {

	std::vector <char>	buffer (BUFFER_SIZE, '\0');
	std::string str;

	int	bytesRead = recv (fd, &buffer [0], buffer.size (), 0);
	if (bytesRead < 0) {
		throw std::runtime_error (RECVERR);
	}
	else if (bytesRead == 0) { // TODO: find if it ever happens??
		log (fd, "End of connexion");
		close (fd);
	}
	else {
		buffer.resize (bytesRead);
		str.assign(&buffer[0]);
		display_buffer (str);
		epollEvents.editSocketInEpoll (fd, EPOLLOUT);
	}
}

void	Server::writeToClient (int fd) {

	std::string message = "Request received";
	if ((send (fd, message.c_str (), message.length (), 0)) < 0) {
		throw std::runtime_error (SENDERR);
	}
	// TODO: delete client?
	close (fd);
}
