#ifndef SERVER_CONTEXT_HPP
# define SERVER_CONTEXT_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "print.hpp"

class ServerContext {

	public:
		ServerContext();
		ServerContext(ServerContext const&);
		ServerContext& operator=(ServerContext const& rhs);
		virtual ~ServerContext();

		size_t										getClientMaxBodySize() const;
		std::map<int, std::string> const &			getErrorPages() const;
		std::vector<std::string> const&				getIndex() const;
		std::string const&							getRoot() const;
		std::map<std::string, std::string> const&	getLocations() const;
		bool										getAutoindex() const;
		std::vector<std::string> const&				getAuthorizedMethods() const;
		std::map<std::string, int> const &			getListen(void) const;
		std::vector<std::string> const &			getServerNames(void) const;

		void	setClientMaxBodySize(std::string const& line);
		void	setErrorPage(std::string const& line);
		void	setIndex(std::string const& line);
		void	setRoot(std::string const& line);
		void	setAutoindex(std::string const& line);
		void	setAuthorizedMethods(std::string const& line);
		void	setLocation(std::stringstream& stream);
		void	setListen(std::string const& ip, int port);
		void	setServerName(std::string const& serverName);

	private:
		bool								_autoindex;
		size_t								_clientMaxBodySize;
		std::string							_root;

		std::map<int, std::string>			_errorPages;
		std::map<std::string, int> 			_listen; // <IP, port>
		std::map<std::string, std::string>	_locations;
		
		std::vector<std::string>			_index;
		std::vector<std::string>			_authorizedMethods;
		std::vector<std::string>			_serverNames;

};

std::ostream &	operator<<(std::ostream & o, ServerContext const & cc);

#endif
