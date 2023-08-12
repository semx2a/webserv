#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <map>

#include "Config.hpp"
#include "Epoll.hpp"
#include "Request.hpp"

typedef std::map <int, std::vector<char> >		chunkRequests_t;
typedef std::map <int, std::vector<char> >::iterator		chunkRequestsIt_t;

class Server {

	public:

		Server ();
		Server (std::string const& conf_file);
		Server (Server const& rhs);
		~Server ();
		Server& operator= (Server const& rhs);

		Config const&		getConfig () const;
		Epoll const&		getEpollEvents () const;

		void				connect ();

	private:

		Config								config;
		Epoll								epollEvents;
		Request								clientRequest;
		chunkRequests_t						chunkRequests;

		void								readFromClient (int fd);
		void								writeToClient (int fd);
		//bool								isRequestEnded (chunkRequests_t::iterator it);
		bool								isRequestEnded (chunkRequestsIt_t it);

};

#endif
