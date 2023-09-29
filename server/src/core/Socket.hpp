#ifndef SOCKET_HPP
# define SOCKET_HPP

// # define DEBUG_SOCKET

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "print.hpp"

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

		int							getFd() const;
		std::string const&			getIp() const;
		int							getPort() const;
		struct sockaddr_in const& 	getServerAddr() const;

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
		uint32_t	_getIntIp(std::string const&);
};

#endif
