#ifndef EPOLL_HPP
# define EPOLL_HPP

#include <arpa/inet.h>
#include <iostream>
#include <vector>

#include "Socket.hpp"

# define BUFFER_SIZE 1024

class Epoll {

	public:

		Epoll (int port); // temp waiting for vector of fds
		Epoll (std::vector <int> ports);
		Epoll (Epoll const& rhs);
		~Epoll ();
		Epoll& operator= (Epoll const& rhs);
		
		void				createEpollEvent ();
		void				editSocketInEpoll (int fd, int eventToWatch); //TODO
		int					waitForConnexions ();
		void				addNewClient (int fd);
		void				readFromClient (int fd);
		void				writeToClient (int fd); //TODO

		int					getServerFd () const { return m_serverFd; } // temp waiting for vector of fds
		struct epoll_event	getReadyEvent (int index) const;


	private:

		int					m_epollFd;
		int					m_serverFd; // temp waiting for vector of fds
		std::vector <int>	m_sockFds;
		struct epoll_event	m_toPoll;
		struct epoll_event	m_events [MAX_EVENTS];
		int					m_clientSocket;
		struct sockaddr_in	m_clientAddress;
		socklen_t			m_clientAddressSize;

		int					pollPort (int port);
		void				addSocketToEpoll (int fd);

};

#endif
