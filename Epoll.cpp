#include "Epoll.hpp"

/**********************************************************************************************************************/
/*										CONSTRUCTORS / DESTRUCTORS													  */
/**********************************************************************************************************************/


/*____________temp waiting for vector of fds__________*/
Epoll::Epoll (int port) {

	createEpollEvent ();
	pollPort (port);
}
/*____________________________________________________*/


Epoll::Epoll (std::vector <int> ports) {

	try {
		for (std::vector <int>::iterator it = ports.begin (); it != ports.end (); it++) {
			_sockFds.push_back (pollPort (*it));
		}
	} catch (const std::exception& e) {
		std::cerr << "Epoll: " << e.what () << std::endl;	
	}
}


Epoll::Epoll (Epoll const& rhs) {

	*this = rhs;
}


Epoll::~Epoll () {}


Epoll& Epoll::operator= (Epoll const& rhs) {

	if (this != &rhs) {
        // TODO
	}
	return *this;
}


/**********************************************************************************************************************/
/*											GETTERS / SETTERS														  */
/**********************************************************************************************************************/


struct epoll_event	Epoll::getReadyEvent (int index) const { 

	return _events [index];
}


/**********************************************************************************************************************/
/*											MEMBER FUNCTIONS														  */
/**********************************************************************************************************************/


void	Epoll::createEpollEvent () {

	_epollFd = epoll_create (10); // TODO : fix number
	if (_epollFd == -1) {
		throw std::runtime_error ("epoll_create1 (): " + (std::string) strerror (errno));
	}
}


int		Epoll::pollPort (int port) {

	Socket newSocket (port);
	newSocket.createSocket ();
	_serverFd = newSocket.getFd ();
	newSocket.setReusable ();
	newSocket.setServerAddr ();
	newSocket.bindSock ();
	newSocket.startListening ();
	addSocketToEpoll (newSocket.getFd ());

	return newSocket.getFd ();
}


void	Epoll::addSocketToEpoll (int fd) {

	std::memset ((char *)&_to_poll, 0, sizeof (_to_poll));

	_to_poll.events = EPOLLIN; 
	_to_poll.data.fd = fd;  
	if (epoll_ctl (_epollFd, EPOLL_CTL_ADD, fd, &_to_poll) == -1) {
		throw std::runtime_error ("epoll_ctl (): " + (std::string) strerror (errno));
	}
}


void	Epoll::editSocketInEpoll () {

	//TODO
}


int		Epoll::waitForConnexions () {

	int numEvents = epoll_wait (_epollFd, _events, MAX_EVENTS, -1);
	if (numEvents == -1) {
		throw std::runtime_error ("epoll_wait (): " + (std::string) strerror (errno));
	}
	return numEvents;
}


void	Epoll::addNewClient (int fd) {

	//if ((_clientSocket = accept (fd, (struct sockaddr*)&_clientAddress, &_clientAddressSize) == -1)) {
	if ((_clientSocket = accept (fd, NULL, NULL)) == -1) {
		throw std::runtime_error ("accept (): " + (std::string) strerror (errno));
	}
	//std::cout << "Nouvelle connexion entrante : " << inet_ntoa (_clientAddress.sin_addr) << std::endl;
	std::cout << "Nouvelle connexion entrante" << std::endl;
	addSocketToEpoll (_clientSocket);
	// TODO : set reusable ?	
}


void	Epoll::readFromClient (int fd) {

	char buffer [BUFFER_SIZE];
	int bytesRead = read (fd, buffer, BUFFER_SIZE - 1);	// note: change for recv
														
	if (bytesRead == -1) {
		close (fd);
		throw std::runtime_error ("read (): " + (std::string) strerror (errno)); // note: A SUPPR A TERME CAR INTERDIT DANS SUJET
	}
	else if (bytesRead == 0) {
		std::cout << "Connexion terminee" << std::endl;
		close (fd);
	}
	else {
		buffer [bytesRead] = '\0';
		std::cout << "Donnees reçues : " << buffer << std::endl;
		send (fd, "END", 3, 0);
	}
}


void	Epoll::writeToClient (int fd) {

	std::cout << "Write to client " << fd << ": TODO" << std::endl;
}


