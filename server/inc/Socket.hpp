#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <cerrno>
#include <cstring>
#include <exception>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

const int MAX_EVENTS = 10;

class Socket {

	public:

		Socket ();
		Socket (int port);
		Socket (std::string conf_file);
		Socket (Socket const& rhs);
		~Socket ();
		Socket& operator= (Socket const& rhs);

		int					getPort () const;
		int					getFd () const;

		void				setReusable ();

	private:

		int					_fd;
		int	const			_port;
		struct sockaddr_in	_serverAddr;

		void				_createSocket ();
		void				_setServerAddr ();
		void				_bindSock ();
		void				_startListening ();
};

#endif
