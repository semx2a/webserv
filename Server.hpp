#ifndef SERVER_HPP
# define SERVER_HPP

#include <map>
#include "Epoll.hpp"

class Server {

	public:

		Server ();
		Server (std::string conf_file);
		Server (Server const& rhs);
		~Server ();
		Server& operator= (Server const& rhs);

		void				connect ();

	private:

		Epoll								m_epollEvents;
		std::map <int, std::vector <char> >	m_chunkRequests;


};

#endif
