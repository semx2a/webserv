#ifndef EPOLL_HPP
# define EPOLL_HPP

//# define DEBUG_EPOLL

#include <algorithm>
#include <iostream>
#include <vector>

#include "utl.hpp"
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
		
		std::map<int, ServerContext> const&		servers() const;
		int										listener() const;
		struct epoll_event const&				readyEvent(int index) const;
		struct epoll_event const&				toPoll() const;

		void	setServers(std::map<int, ServerContext> const&);
		void	setListener(int);
		void	setReadyEvent(int, struct epoll_event const&);
		void	setToPoll(struct epoll_event const&);

		int			waitForConnexions();
		bool		isNewClient(int fd) const;
		void		addSocketToEpoll(int fd);
		void		editSocketInEpoll(int fd, int eventToWatch); //TODO

	private:

		std::map<int, ServerContext>	_servers;
		int								_listener;
		struct epoll_event				_toPoll;
		struct epoll_event				_events[MAX_EVENTS];

		int					_pollPort(std::string const& ip, int port);
		void				_createEpollEvent();
};

#endif
