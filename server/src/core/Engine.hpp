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

		Epoll const&						getEpollEvents() const;
		std::map<int, ServerContext> const&	getServersContexts() const;
		std::map<int, Buffer> const&		getBuffersMap() const;
		std::map<int, Request> const&		getRequestsMap() const;

		void	setEpollEvents(Epoll const&);
		void	setServersContexts(std::map<int, ServerContext> const&);
		void	setBuffersMap(std::map<int, Buffer> const&);
		void	setRequestsMap(std::map<int, Request> const&);

		void	connect();

	private:

		Engine();

		Epoll								_epollEvents;
		std::map<int, ServerContext>		_serverContextsMap;
		std::map<int, Buffer>				_buffersMap;
		std::map<int, Request>				_requestsMap;

		void				_addNewClient(int);
		void				_readFromClient(int);
		void				_writeToClient(int);
		void				_handleBuffer(int);
		void				_closeSocket(int);

};

#endif
