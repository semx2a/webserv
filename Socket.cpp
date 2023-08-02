#include "Socket.hpp"

/**********************************************************************************************************************/
/*										CONSTRUCTORS / DESTRUCTORS													  */
/**********************************************************************************************************************/


Socket::Socket (int port) : m_port (port) {

	createSocket ();
	setReusable ();
	setServerAddr ();
	bindSock ();
	startListening ();
}


Socket::Socket (Socket const& rhs) : m_port (rhs.getPort ()) {

	*this = rhs;
}


Socket::~Socket () {}


Socket& Socket::operator= (Socket const& rhs) {

	if (this != &rhs) {
        // TODO
	}
	return *this;
}


/**********************************************************************************************************************/
/*											GETTERS / SETTERS														  */
/**********************************************************************************************************************/


int		Socket::getPort () const { return m_port; }

int		Socket::getFd () const { return m_sockFd; }


/**********************************************************************************************************************/
/*											MEMBER FUNCTIONS														  */
/**********************************************************************************************************************/


void	Socket::createSocket () {

	m_sockFd = socket (AF_INET, SOCK_STREAM, 0);
	if (m_sockFd == -1)
		throw std::runtime_error ("socket (): " + (std::string) strerror (errno));
}


void	Socket::setReusable () {

	int on = 1;
	setsockopt (m_sockFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof (int));
}


void	Socket::setServerAddr () {

	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons (m_port);
	m_serverAddr.sin_addr.s_addr = htonl (INADDR_ANY);
}


void	Socket::bindSock () {

	if (bind (m_sockFd, (struct sockaddr*)&m_serverAddr, sizeof (m_serverAddr)) == -1) {
		close (m_sockFd);
		// TODO : close general des sockets
		throw std::runtime_error ("bind (): " + (std::string) strerror (errno));
	}
}


void	Socket::startListening () {

	if (listen (m_sockFd, 5) == -1) {
		close (m_sockFd);
		throw std::runtime_error ("listen (): " + (std::string) strerror (errno));
	}
}

