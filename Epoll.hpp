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
		void				editSocketInEpoll (); //TODO
		int					waitForConnexions ();
		void				addNewClient (int fd);
		void				readFromClient (int fd);
		void				writeToClient (int fd); //TODO

		int					getServerFd () const { return _serverFd; } // temp waiting for vector of fds
		struct epoll_event	getReadyEvent (int index) const;


	private:

		int					_epollFd;
		int					_serverFd; // temp waiting for vector of fds
		std::vector <int>	_sockFds;
		struct epoll_event	_to_poll;
		struct epoll_event	_events [MAX_EVENTS];
		int					_clientSocket;
		struct sockaddr_in	_clientAddress;
		socklen_t			_clientAddressSize;

		int					pollPort (int port);
		void				addSocketToEpoll (int fd);

};

#endif
