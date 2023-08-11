#ifndef EPOLL_HPP
# define EPOLL_HPP

#include "display.hpp"
#include "extern_libs.hpp"
#include "Socket.hpp"

# define BUFFER_SIZE 1024

# define ACCEPTERR "accept (): failed"
# define ECREATERR "epoll_create (): failed"
# define ECTLERR "epoll_ctl (): failed"
# define EWAITERR "epoll_wait (): failed"
# define RECVERR "recv (): failed"
# define SENDERR "send (): failed"

class Epoll {

	public:

		Epoll ();
		Epoll (std::vector <int>& ports);
		Epoll (Epoll const& rhs);
		~Epoll ();
		Epoll& operator= (Epoll const& rhs);
		
		struct epoll_event const&	getReadyEvent (int index) const;
		std::vector <int> const&	getServersFds () const;
		bool						isNewClient (int fd); 

		int					waitForConnexions ();
		void				addNewClient (int fd);
		std::vector <char>	receiveBuffer (int fd);
		void				editSocketInEpoll (int fd, int eventToWatch); //TODO

	private:

		int					epollFd;
		std::vector <int>	serversFds;
		struct epoll_event	toPoll;
		struct epoll_event	events [MAX_EVENTS];

		int					pollPort (int port);
		void				addSocketToEpoll (int fd);
		void				createEpollEvent ();
};

#endif
