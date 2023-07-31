#include "Server.hpp"

/**********************************************************************************************************************/
/*										CONSTRUCTORS / DESTRUCTORS													  */
/**********************************************************************************************************************/
/*____________temp waiting for vector of fds__________*/
Server::Server () : _epollEvents (8080) {}

/*
Server::Server (std::string config_file) {
}
*/

Server::Server (Server const& rhs) : _epollEvents (8080) {
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
	try {
		while (true)
		{
			_numEvents = _epollEvents.waitForConnexions ();

			// Parcourir tous les evenements
			for (int i = 0; i < _numEvents; ++i)
			{
				_event = _epollEvents.getReadyEvent (i);
				if ((_event.events & EPOLLERR) || (_event.events & EPOLLHUP) || (_event.events & EPOLLRDHUP)) {
					// TODO : delete corresponding nodes in client map
					close (_event.data.fd);
				}
				else if (_event.data.fd == _epollEvents.getServerFd ()) // temp waiting for vector of fds
					_epollEvents.addNewClient (_event.data.fd);
				else if (_event.events & EPOLLIN) 
					_epollEvents.readFromClient (_event.data.fd);
				else if (_event.events & EPOLLOUT)
					_epollEvents.writeToClient (_event.data.fd);
			}
		}
	}
	catch (std::exception& e) {
		std::cerr << "Server: Error: " << e.what () << std::endl;
	}
}
