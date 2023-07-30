#include <cstring>
#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

const int MAX_EVENTS = 10;

class Socket {

	public:
		Socket (int port);

		void	createSocket ();
		void	setReusable ();
		void	setServerAddr ();
		void	bindSock ();
		void	startListening ();

		int		getFd () const { return _sockFd; } //TODO implement in .cpp

	private:
		int					_sockFd;
		int					_port;
		struct sockaddr_in	_serverAddr;
		struct epoll_event	_events [MAX_EVENTS];
};

class Epoll {

	public:
		Epoll ();
		Epoll (std::vector <int> ports);
		Epoll (Epoll const& rhs);
		~Epoll ();
		Epoll& operator= (Epoll const& rhs);
		
		void	createEpollEvent ();
		void	editSocketInEpoll ();

		// TODO : exception ?

	private:
		int		pollPort (int port);
		void	addSocketToEpoll (int fd);

		int					_epollFd;
		std::vector <int>	_sockFds;

};

