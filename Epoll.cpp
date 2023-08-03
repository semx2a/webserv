#include "Epoll.hpp"
#include <algorithm>

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Epoll::Epoll () {}

Epoll::Epoll (std::vector <int>& ports) {

	try {
		createEpollEvent ();
		for (std::vector <int>::iterator it = ports.begin (); it != ports.end (); it++) {
			sockFds.push_back (pollPort (*it));
		}
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR: " << e.what () << std::endl;	
	}
}

Epoll::Epoll (Epoll const& rhs) {

	*this = rhs;
}

Epoll& Epoll::operator= (Epoll const& rhs) {

	if (this != &rhs) {
        // TODO
	}
	return *this;
}

Epoll::~Epoll () {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

struct epoll_event const&	Epoll::getReadyEvent (int index) const {

	return events [index];
}

std::vector <int> const&	Epoll::getSockFds () const { 

	return sockFds;
}

bool	Epoll::isSockFd (int fd) {

	std::vector <int>::iterator it;

	it = std::find (sockFds.begin (), sockFds.end (), fd);
	return (it != sockFds.end ());
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CREATION

void	Epoll::createEpollEvent () {

	epollFd = epoll_create (10); // TODO : fix number
	if (epollFd < 0) {
		throw std::runtime_error (ECREATERR);
	}
}

int		Epoll::pollPort (int port) {

	Socket newSocket (port);
	int fd = newSocket.getFd ();
	addSocketToEpoll (fd);

	return fd;
}

void	Epoll::addSocketToEpoll (int fd) {

	std::memset ((char *)&toPoll, 0, sizeof (toPoll));
	toPoll.events = EPOLLIN; 
	toPoll.data.fd = fd;  
	if (epoll_ctl (epollFd, EPOLL_CTL_ADD, fd, &toPoll) < 0) {
		throw std::runtime_error (ECTLERR);
	}
}

void	Epoll::addNewClient (int fd) {

	int	clientSocket = accept (fd, NULL, NULL);
	if (clientSocket < 0) {
		throw std::runtime_error (ACCEPTERR);
	}
	std::cout << "\nNouvelle connexion entrante\n" << std::endl;
	addSocketToEpoll (clientSocket);
	// TODO : set reusable ?	
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MODIFICATION

void	Epoll::editSocketInEpoll (int fd, int eventToWatch) {

	std::memset ((char *)&toPoll, 0, sizeof (toPoll));
	toPoll.events = eventToWatch; 
	toPoll.data.fd = fd;  
	if (epoll_ctl (epollFd, EPOLL_CTL_MOD, fd, &toPoll) < 0) {
		throw std::runtime_error (ECTLERR);
	}
}


int		Epoll::waitForConnexions () {

	int numEvents = epoll_wait (epollFd, events, MAX_EVENTS, -1);
	if (numEvents < 0) {
		throw std::runtime_error (EWAITERR);
	}
	return numEvents;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::I/O OPERATIONS

void	Epoll::readFromClient (int fd) {

	std::vector <char>	buffer (BUFFER_SIZE, '\0');

	int	bytesRead = recv (fd, &buffer [0], buffer.size (), 0);
	if (bytesRead < 0) {
		throw std::runtime_error (RECVERR);
	}
	else if (bytesRead == 0) {
		std::cout << "Connexion terminee" << std::endl;
		close (fd);
	}
	else {
		buffer.resize (bytesRead);
		std::cout << "Donnees reçues : " << &buffer[0] << std::endl; // TODO : handle request
		editSocketInEpoll (fd, EPOLLOUT);
	}
}

void	Epoll::writeToClient (int fd) {

	std::string message = "Request received";
	if ((send (fd, message.c_str (), message.length (), 0)) < 0) {
		throw std::runtime_error (SENDERR);
	}
	close (fd);
}


