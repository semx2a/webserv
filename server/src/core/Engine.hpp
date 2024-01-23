#ifndef ENGINE_HPP
# define ENGINE_HPP

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

		Epoll const&						epoll(void) const;
		std::map<int, ServerContext> const&	serverContexts(void) const;
		std::map<int, Buffer> const&		buffers(void) const;
		std::map<int, Request> const&		requests(void) const;

		void	setEpollEvents(Epoll const&);
		void	setServersContexts(std::map<int, ServerContext> const&);
		void	setBuffers(std::map<int, Buffer> const&);
		void	setRequests(std::map<int, Request> const&);

		void	connexionLoop(void);

	private:

		Engine(void);

		Epoll								_epoll;
		std::map<int, ServerContext>		_serverContexts;
		std::map<int, Buffer>				_buffers;
		std::map<int, Request>				_requests;
		std::map<int, HttpStatus>			_status;

		void				_addNewClient(int);
		void				_readFromClient(int);
		void				_writeToClient(int);
		void				_handleBuffer(int);
		void				_endConnexion(int);

};

#endif
