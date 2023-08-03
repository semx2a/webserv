#include "Server.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Server::Server (Config& config) : epollEvents (config.getPorts ()) {
}

Server::Server (Server const& rhs) {

	*this = rhs;
}
/*____________________________________________________*/


Server::~Server () {}


Server& Server::operator= (Server const& rhs) {

	if (this != &rhs) {
        // TODO
	}
	return *this;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

Config const&		Server::getConfig () const { return config; }

Epoll const&		Server::getEpollEvents () const { return epollEvents; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MEMBER FUNCTIONS


void	Server::connect () {

	int					nb_events;
	struct epoll_event	event;

	try {

		while (true) {

			nb_events = epollEvents.waitForConnexions ();
			for (int i = 0; i < nb_events; ++i) {

				event = epollEvents.getReadyEvent (i);
				if ((event.events & EPOLLERR) || (event.events & EPOLLHUP) || (event.events & EPOLLRDHUP)) {
					close (event.data.fd);// TODO when map of clients : delete corresponding nodes in client map
				}
				else if (epollEvents.isSockFd (event.data.fd)) { // temp waiting for vector of fds 
					epollEvents.addNewClient (event.data.fd);
				}
				else if (event.events & EPOLLIN) {
					epollEvents.readFromClient (event.data.fd);
				}
				else if (event.events & EPOLLOUT) {
					epollEvents.writeToClient (event.data.fd);
					std::cout << "Waiting for new connexion..." << std::endl;
				}
			}
		}
	}
	catch (std::exception& e) {

		std::cerr << "ERROR: " << e.what () << std::endl;
	}
}
