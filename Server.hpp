#include <cerrno>

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
		Epoll				_epollEvents;
		int					_numEvents;
		struct epoll_event	_event;


};
