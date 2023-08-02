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
			m_sockFds.push_back (pollPort (*it));
		}
	}
	catch (const std::exception& e) {
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

	return m_events [index];
}


/**********************************************************************************************************************/
/*											MEMBER FUNCTIONS														  */
/**********************************************************************************************************************/


void	Epoll::createEpollEvent () {

	m_epollFd = epoll_create (10); // TODO : fix number
								   //
	if (m_epollFd == -1) {
		throw std::runtime_error ("epoll_create1 (): " + (std::string) strerror (errno));
	}
}


int		Epoll::pollPort (int port) {

	Socket newSocket (port);
	m_serverFd = newSocket.getFd ();
	addSocketToEpoll (newSocket.getFd ());

	return newSocket.getFd ();
}


void	Epoll::addSocketToEpoll (int fd) {

	std::memset ((char *)&m_toPoll, 0, sizeof (m_toPoll));

	m_toPoll.events = EPOLLIN; 
	m_toPoll.data.fd = fd;  
	if (epoll_ctl (m_epollFd, EPOLL_CTL_ADD, fd, &m_toPoll) == -1) {
		throw std::runtime_error ("epoll_ctl (): " + (std::string) strerror (errno));
	}
}


void	Epoll::editSocketInEpoll (int fd, int eventToWatch) {

	std::memset ((char *)&m_toPoll, 0, sizeof (m_toPoll));

	m_toPoll.events = eventToWatch; 
	m_toPoll.data.fd = fd;  
	if (epoll_ctl (m_epollFd, EPOLL_CTL_MOD, fd, &m_toPoll) == -1) {
		throw std::runtime_error ("epoll_ctl (): " + (std::string) strerror (errno));
	}
}


int		Epoll::waitForConnexions () {

	int numEvents = epoll_wait (m_epollFd, m_events, MAX_EVENTS, -1);
	if (numEvents == -1) {
		throw std::runtime_error ("epoll_wait (): " + (std::string) strerror (errno));
	}
	return numEvents;
}


void	Epoll::addNewClient (int fd) {

	//if ((m_clientSocket = accept (fd, (struct sockaddr*)&_clientAddress, &_clientAddressSize) == -1)) {
	if ((m_clientSocket = accept (fd, NULL, NULL)) == -1) {
		throw std::runtime_error ("accept (): " + (std::string) strerror (errno));
	}
	//std::cout << "Nouvelle connexion entrante : " << inet_ntoa (_clientAddress.sin_addr) << std::endl;
	std::cout << "\nNouvelle connexion entrante\n" << std::endl;
	addSocketToEpoll (m_clientSocket);
	// TODO : set reusable ?	
}


void	Epoll::readFromClient (int fd) {

	std::vector <char>	buffer (BUFFER_SIZE, '\0');
	int					bytesRead;

	if ((bytesRead = recv (fd, &buffer [0], buffer.size (), 0)) < 0) {
		throw std::runtime_error ("recv (): " + (std::string) strerror (errno)); // note: A SUPPR A TERME CAR INTERDIT DANS SUJET
	}
	else if (bytesRead == 0) {
		std::cout << "Connexion terminee" << std::endl;
		close (fd);
	}
	else {
		buffer.resize (bytesRead);
		std::cout << "Donnees reçues : " << &buffer[0] << std::endl;
		editSocketInEpoll (fd, EPOLLOUT);
	}
}


void	Epoll::writeToClient (int fd) {

	std::string message = "Request received";
	send (fd, message.c_str (), message.length (), 0);
	close (fd);
}


