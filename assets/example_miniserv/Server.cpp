#include "Engine.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Engine::Engine () :	config (), 
					epollEvents (config.ports ()) {
}

Engine::Engine (std::string const& conf_file) :	config (conf_file), 
												epollEvents (config.ports ()) {
}

Engine::Engine (Engine const& rhs) {

	*this = rhs;
}

Engine::~Engine () {}


Engine& Engine::operator= (Engine const& rhs) {

	if (this != &rhs) {
        // TODO
	}
	return *this;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

Config const&	Engine::getConfig () const { return config; }

Epoll const&	Engine::epoll () const { return epollEvents; }


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MEMBER FUNCTIONS


void	Engine::connect () {

	struct epoll_event	event;
	int					nb_events;

	try {

		while (true) {

			nb_events = epollEvents.waitForConnexions ();
			for (int i = 0; i < nb_events; ++i) {

				event = epollEvents.readyEvent (i);
				if ((event.events & EPOLLERR) || (event.events & EPOLLHUP) || (event.events & EPOLLRDHUP)) {
					close (event.data.fd);// TODO : delete corresponding nodes in client map
				}
				else if (epollEvents.isNewClient (event.data.fd)) {
					epollEvents.addNewClient (event.data.fd);
				}
				else if (event.events & EPOLLIN) {
					epollEvents.readFromClient (event.data.fd);
				}
				else if (event.events & EPOLLOUT) {
					epollEvents.writeToClient (event.data.fd);
				}
			}
		}
	}
	catch (std::exception& e) {

		std::cerr << "ERROR: " << e.what () << std::endl;
	}
}
