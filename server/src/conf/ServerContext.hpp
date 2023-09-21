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

		bool										getAutoindex() const;
		size_t										getClientMaxBodySize() const;
		std::string const&							getRoot() const;

		std::map<std::string, int> const&			getListen(void) const;
		std::map<int, std::string> const&			getErrorPages() const;
		std::map<std::string, std::string> const&	getLocations() const;

		std::vector<std::string> const&				getIndex() const;
		std::vector<std::string> const&				getAuthorizedMethods() const;
		std::vector<std::string> const&				getServerNames(void) const;

		void	setAutoindex(bool autoindex);
		void	setClientMaxBodySize(size_t size);
		void	setRoot(std::string const& root);

		void	setListen(std::string const& ip, int port);
		void	setErrorPage(std::map<int, std::string> const& errorPages);
		void	setLocation(std::string const& location, std::string const& root);

		void	setIndex(std::vector<std::string> const& index);
		void	setAuthorizedMethods(std::vector<std::string> const& authorizedMethods);
		void	setServerNames(std::vector<std::string> const& serverNames);

	private:
		bool								_autoindex;
		size_t								_clientMaxBodySize;
		std::string							_root;

		std::map<std::string, int> 			_listen; // <IP, port>
		std::map<int, std::string>			_errorPages;
		std::map<std::string, std::string>	_locations; // special : location context. Class?
		
		std::vector<std::string>			_index;
		std::vector<std::string>			_authorizedMethods;
		std::vector<std::string>			_serverNames;

};

std::ostream &	operator<<(std::ostream & o, ServerContext const & cc);

#endif
