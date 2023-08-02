#include "Server.hpp"

/**********************************************************************************************************************/
/*										CONSTRUCTORS / DESTRUCTORS													  */
/**********************************************************************************************************************/


/*___________m_temp waiting for vector of fds__________*/
Server::Server () : m_epollEvents (8080) {}

/*
Server::Server (std::string configm_file) {
}
*/


Server::Server (Server const& rhs) : m_epollEvents (8080) {

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


/**********************************************************************************************************************/
/*											GETTERS / SETTERS														  */
/**********************************************************************************************************************/


/**********************************************************************************************************************/
/*											MEMBER FUNCTIONS														  */
/**********************************************************************************************************************/


void	Server::connect () {

	int					nb_events;
	struct epoll_event	event;

	try {

		while (true) {

			nb_events = m_epollEvents.waitForConnexions ();

			for (int i = 0; i < nb_events; ++i) {

				event = m_epollEvents.getReadyEvent (i);

				if ((event.events & EPOLLERR) || (event.events & EPOLLHUP) || (event.events & EPOLLRDHUP)) {
					close (event.data.fd);// TODO when map of clients : delete corresponding nodes in client map
				}
				else if (event.data.fd == m_epollEvents.getServerFd ()) { // temp waiting for vector of fds 
					m_epollEvents.addNewClient (event.data.fd);
				}
				else if (event.events & EPOLLIN) {
					m_epollEvents.readFromClient (event.data.fd);
				}
				else if (event.events & EPOLLOUT) {
					m_epollEvents.writeToClient (event.data.fd);
					std::cout << "Waiting for new connexion..." << std::endl;
				}
			}
		}
	}
	catch (std::exception& e) {

		std::cerr << "Server: Error: " << e.what () << std::endl;
	}
}
