#ifndef SERVER_HPP
# define SERVER_HPP

# define DEBUG

#include <exception>
#include <iostream>
#include <map>

#include "Buffer.hpp"
#include "ServerContext.hpp"
#include "Epoll.hpp"
#include "Request.hpp"
#include "Response.hpp"

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
		std::map<int, Buffer>				_buffersMap;

		void				_addNewClient(int serverFd);
		void				_readFromClient(int clientFd);
		void				_writeToClient(int clientFd);
		void				_handleBuffer(int clientFd);
		void				_closeSocket(int fd);

};

#endif
