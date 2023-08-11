#ifndef SERVER_HPP
# define SERVER_HPP

#include <map>
#include "Config.hpp"
#include "Epoll.hpp"
#include "Request.hpp"

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
		std::map <int, std::vector <char> >	chunkRequests;

		void								readFromClient (int fd);
		void								writeToClient (int fd);

};

#endif
