#ifndef SERVER_HPP
# define SERVER_HPP

# define DEBUG

#include <iostream>
#include <map>

#include "Config.hpp"
#include "Epoll.hpp"
#include "Request.hpp"

typedef std::map <int, std::vector<char> >		clientData_t;
typedef std::map <int, std::vector<char> >::iterator		clientDataIt_t;

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
		clientData_t						clientData;

		void								readFromClient (int clientFd);
		void								writeToClient (int clientFd);
		bool								isRequestEnded (int clientFd);
		void								handleRequest (int clientFd);
		void								endClientConnexion (int clientFd);

};

#endif
