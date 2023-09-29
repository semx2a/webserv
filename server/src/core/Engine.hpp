#ifndef ENGINE_HPP
# define ENGINE_HPP

//# define DEBUG_ENGINE

#include <exception>
#include <iostream>
#include <map>

#include "Buffer.hpp"
#include "ServerContext.hpp"
#include "Epoll.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ResponseHandler.hpp"
#include "ResponseBuilder.hpp"

class Engine {

	public:
		Engine(std::vector<ServerContext> const& serversContexts);
		Engine(std::string const& conf_file);
		Engine(Engine const& rhs);
		~Engine();
		Engine& operator=(Engine const& rhs);

		Epoll const&						getEpollEvents() const;
		std::map<int, ServerContext> const&	getServersContexts() const;
		std::map<int, Buffer> const&		getBuffers() const;
		std::map<int, Request> const&		getRequests() const;

		void	setEpollEvents(Epoll const&);
		void	setServersContexts(std::map<int, ServerContext> const&);
		void	setBuffers(std::map<int, Buffer> const&);
		void	setRequests(std::map<int, Request> const&);

		void	connect();

	private:

		Engine();

		Epoll								_epoll;
		std::map<int, ServerContext>		_serverContexts;
		std::map<int, Buffer>				_buffers;
		std::map<int, Request>				_requests;

		void				_addNewClient(int);
		void				_readFromClient(int);
		void				_writeToClient(int);
		void				_handleBuffer(int);
		void				_closeSocket(int);

};

#endif
