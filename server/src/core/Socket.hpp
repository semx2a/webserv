#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <arpa/inet.h>
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

		Socket(int port);
		Socket(std::string const& ip, int port);
		Socket(std::string conf_file);
		Socket(Socket const& rhs);
		~Socket();
		Socket& operator=(Socket const& rhs);

		int					getPort() const;
		int					getFd() const;

		void				setReusable();

	private:

		Socket();

		int					_fd;
		std::string			_ip;
		int					_port;
		struct sockaddr_in	_serverAddr;

		void				_createSocket();
		void				_setServerAddr();
		void				_bindSock();
		void				_startListening();
};

#endif
