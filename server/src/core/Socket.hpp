#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "utl.hpp"

extern "C" {
	#include <netinet/in.h>
	#include <sys/epoll.h>
	#include <sys/socket.h>
	#include <unistd.h>
}

const int MAX_EVENTS = 10;

class Socket {

	public:

		Socket(std::string const& ip, int port);
		Socket(Socket const& rhs);
		~Socket();
		Socket& operator=(Socket const& rhs);

		int							fd() const;
		std::string const&			ip() const;
		int							port() const;
		struct sockaddr_in const& 	serverAddr() const;

		void	setFd(int);
		void	setIp(std::string const&);
		void	setPort(int);
		void	setServerAddr(struct sockaddr_in const&);

		void	setReusable();

	private:

		Socket();

		int					_fd;
		std::string			_ip;
		int					_port;
		struct sockaddr_in	_serverAddr;

		void		_createSocket();
		void		_setServerAddr();
		void		_bindSock();
		void		_startListening();
		uint32_t	_intIp(std::string const&);
};

#endif
