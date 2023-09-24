#ifndef SERVER_HPP
# define SERVER_HPP

# define DEBUG

#include <iostream>
#include <map>

#include "ClientData.hpp"
#include "Config.hpp"
#include "Epoll.hpp"
#include "Request.hpp"

typedef std::map<int, ClientData>				clientDataMap_t;

class Server {

	public:

		Server();
		Server(std::string const& conf_file);
		Server(Server const& rhs);
		~Server();
		Server& operator=(Server const& rhs);

		Config const&		getConfig() const;
		Epoll const&		getEpollEvents() const;

		void				connect();

	private:

		Config				_config;
		Epoll				_epollEvents;
		Request				_clientRequest;
		clientDataMap_t		_clientDataMap;

		void				_readFromClient(int clientFd);
		void				_writeToClient(int clientFd);
		void				_handleClientData(int clientFd);
		void				_closeSocket(int fd);

};

#endif
