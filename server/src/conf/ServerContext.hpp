#ifndef SERVER_CONTEXT_HPP
# define SERVER_CONTEXT_HPP

#include <map>
#include <vector>

#include "print.hpp"
#include "LocationContext.hpp"

class LocationContext;

class ServerContext {

	public:
		ServerContext();
		ServerContext(ServerContext const&);
		ServerContext& operator=(ServerContext const& rhs);
		~ServerContext();

		bool										getAutoindex() const;
		bool										getCgi() const;
		size_t										getMaxBodySize() const;
		std::string const&							getRoot() const;

		std::map<std::string, int> const&				getListen(void) const;
		std::map<int, std::string> const&				getErrorPages() const;
		std::map<std::string, LocationContext> const&	getLocations() const;

		std::vector<std::string> const&				getIndex() const;
		std::vector<std::string> const&				getAuthorizedMethods() const;
		std::vector<std::string> const&				getServerNames(void) const;

		void	setAutoindex(bool autoindex);
		void	setCgi(bool cgi);
		void 	setMaxBodySize(size_t size);
		void	setRoot(std::string const& root);

		void	setListen(std::string const& ip, int port);
		void	addErrorPage(int code, std::string const& path);
		void	setErrorPages(std::map<int, std::string> const& errorPages);
		void	addLocation(std::string const& location, LocationContext const& locationContext);
		void	setLocations(std::map<std::string, LocationContext> locationsMap);

		void	setIndex(std::vector<std::string> const& index);
		void	setAuthorizedMethods(std::vector<std::string> const& authorizedMethods);
		void	setServerNames(std::vector<std::string> const& serverNames);

	private:
		bool								_autoindex;
		bool								_cgi;
		size_t								_maxBodySize;
		std::string							_root;

		std::map<std::string, int> 				_listen; // <IP, port>
		std::map<int, std::string>				_errorPages;
		std::map<std::string, LocationContext>	_locations; // TODO: change to map<string, LocationContext>
		
		std::vector<std::string>			_index;
		std::vector<std::string>			_authorizedMethods;
		std::vector<std::string>			_serverNames;

};

std::ostream &	operator<<(std::ostream & o, ServerContext const & cc);

#endif
