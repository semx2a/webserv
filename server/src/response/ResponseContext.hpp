#ifndef RESPONSECONTEXT_HPP
# define RESPONSECONTEXT_HPP

# include <iostream>

# include "Request.hpp"
# include "ServerContext.hpp"
# include "utl.hpp"

typedef std::map<std::string, LocationContext>::const_iterator	t_locationIterator;
typedef std::map<std::string, ServerContext>::const_iterator	t_serverIterator;

class ResponseContext {

	public:

		ResponseContext(Request const&, ServerContext const&);
		ResponseContext(ResponseContext const&);
		~ResponseContext();
		ResponseContext& operator=(ResponseContext const&);

		// :::::::::::::::::::::::::::: ACCESSORS
		Request const&						request(void) const;
		ServerContext const&				serverContext(void) const;
		std::string const&					target(void) const;
		std::string const&					path(void) const;
		LocationContext const&				location(void) const;
		std::string const&					root(void) const;
		bool								alias(void) const;
		std::string const&					uploadFolder(void) const;
		std::vector<std::string> const&		index(void) const;
		std::map<int, std::string> const&	errorPages(void) const;
		std::string const&					autoindex(void) const;
		size_t 								maxBodySize(void) const;
		std::vector<std::string> const&		authorizedMethods(void) const;
		bool								isCgi(void) const;
		bool								isUpload(void) const;

	private:
	 	Request const&						_request;
		ServerContext const&				_serverContext;

		std::string							_target;
		std::string							_path; // += root
		LocationContext						_location;
		std::string							_root;
		bool								_alias;
		std::string							_uploadFolder; // += root
		std::vector<std::string>			_index; // += root
		std::map<int, std::string>			_errorPages; // += root
		std::string							_autoindex;
		size_t								_maxBodySize;
		std::vector<std::string>			_authorizedMethods;
		bool								_isCgi;
		bool								_isUpload;
		

		void		_locationDirectives(void);
		void		_serverDirectives(void);
};

std::ostream&	operator<<(std::ostream& o, ResponseContext const& rhs);

#endif