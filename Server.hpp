#ifndef SERVER_HPP
# define SERVER_HPP

#include <map>
#include "Config.hpp"
#include "Epoll.hpp"

class Server {

	public:

		Server ();
		Server (Config& config);
		Server (Server const& rhs);
		~Server ();
		Server& operator= (Server const& rhs);

		Config const&		getConfig () const;
		Epoll const&		getEpollEvents () const;

		void				connect ();

	private:

		Config								config;
		Epoll								epollEvents;
		std::map <int, std::vector <char> >	chunkRequests;


};

#endif
