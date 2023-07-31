#ifndef SOCKET_HPP
# define SOCKET_HPP

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

		int		getPort () const;

		void	createSocket ();
		void	setReusable ();
		void	setServerAddr ();
		void	bindSock ();
		void	startListening ();

		int		getFd () const { return _sockFd; } //TODO implement in .cpp

	private:
		int					_sockFd;
		int	const			_port;
		struct sockaddr_in	_serverAddr;
};

#endif
