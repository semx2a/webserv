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

class Engine {

	public:
		Engine(std::vector<ServerContext> const& serversContexts);
		Engine(std::string const& conf_file);
		Engine(Engine const& rhs);
		~Engine();
		Engine& operator=(Engine const& rhs);

		std::vector<ServerContext> const&	getServerContexts() const;
		Epoll const&						getEpollEvents() const;

		void	connect();

	private:

		//Engine();

		std::vector<ServerContext>			_serversContexts;
		Epoll								_epollEvents;
		Request								_clientRequest;
		clientDataMap_t						_clientDataMap;

		void				_addNewClient(int serverFd);
		void				_readFromClient(int clientFd);
		void				_writeToClient(int clientFd);
		void				_handleClientData(int clientFd);
		void				_closeSocket(int fd);

};

#endif
