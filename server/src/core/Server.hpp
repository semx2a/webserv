#ifndef SERVER_HPP
# define SERVER_HPP

# define DEBUG

#include <exception>
#include <iostream>
#include <map>

#include "ClientData.hpp"
#include "ServerContext.hpp"
#include "Epoll.hpp"
#include "Request.hpp"
#include "Response.hpp"

typedef std::map<int, ClientData>				clientDataMap_t;

class Server {

	public:
		Server(std::vector<ServerContext> const& serverContexts);
		Server(std::string const& conf_file);
		Server(Server const& rhs);
		~Server();
		Server& operator=(Server const& rhs);

		std::vector<ServerContext> const&	getServerContexts() const;
		Epoll const&						getEpollEvents() const;

		void	connect();

	private:

		//Server();

		std::vector<ServerContext> const&	_serverContexts;
		Epoll								_epollEvents;
		Request								_clientRequest;
		clientDataMap_t						_clientDataMap;

		void				_readFromClient(int clientFd);
		void				_writeToClient(int clientFd);
		void				_handleClientData(int clientFd);
		void				_closeSocket(int fd);

};

#endif
