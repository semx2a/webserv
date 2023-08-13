#include "../inc/Epoll.hpp"
#include <algorithm>


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Epoll::Epoll () {}

Epoll::Epoll (std::vector <int>& ports) {

	try {
		createEpollEvent ();
		for (std::vector <int>::iterator it = ports.begin (); it != ports.end (); it++) {
			listenFds.push_back (pollPort (*it));
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

std::vector <int> const&	Epoll::getServersFds () const { 

	return listenFds;
}

bool	Epoll::isNewClient (int fd) {

	std::vector <int>::iterator it;

	it = std::find (listenFds.begin (), listenFds.end (), fd);
	return (it != listenFds.end ());
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

	int	clientSocket;

	try {
		clientSocket = accept (fd, NULL, NULL);
		if (clientSocket < 0) {
			throw std::runtime_error (ACCEPTERR);
		}
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR: " << e.what () << std::endl;
	}
	addSocketToEpoll (clientSocket);
	log (clientSocket, "New request");
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
	display_wait ();
	return numEvents;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::I/O OPERATIONS

/*
std::vector <char>	Epoll::receiveBuffer (int fd) {

	std::vector <char>	buffer (BUFFER_SIZE, '\0');

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
	}
	return buffer;
}
*/


/*std::string	Epoll::receiveBuffer (int fd) {

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
	}
	return str;
}*/

