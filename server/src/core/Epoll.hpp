#ifndef EPOLL_HPP
# define EPOLL_HPP

#include <algorithm>
#include <arpa/inet.h>
#include <iostream>
#include <vector>

#include "print.hpp"
#include "Socket.hpp"
#include "ServerContext.hpp"

# define BUFFER_SIZE 1024

# define ACCEPTERR "accept(): failed"
# define ECREATERR "epoll_create(): failed"
# define ECTLERR "epoll_ctl(): failed"
# define EWAITERR "epoll_wait(): failed"
# define RECVERR "recv(): failed"
# define SENDERR "send(): failed"

class Epoll {

	public:

		Epoll();
		Epoll(std::vector<ServerContext> const& serversContexts);
		Epoll(Epoll const& rhs);
		~Epoll();
		Epoll& operator=(Epoll const& rhs);
		
		struct epoll_event const&				getReadyEvent(int index) const;
		std::map<int, ServerContext> const&		getServers() const;
		bool									isNewClient(int fd); 

		int					waitForConnexions();
		void				addSocketToEpoll(int fd);
		void				editSocketInEpoll(int fd, int eventToWatch); //TODO

	private:

		//std::vector<int>				_listenFds;
		std::map<int, ServerContext>	_servers;
		int								_listener;
		struct epoll_event				_toPoll;
		struct epoll_event				_events[MAX_EVENTS];

		int					_pollPort(std::string const& ip, int port);
		void				_createEpollEvent();
};

#endif
