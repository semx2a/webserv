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
		Request const&						request() const;
		ServerContext const&				serverContext() const;
		std::string const&					target() const;
		std::string const&					path() const;
		LocationContext const&				location() const;
		std::string const&					root() const;
		bool								alias() const;
		std::string const&					uploadFolder() const;
		std::vector<std::string> const&		index() const;
		std::map<int, std::string> const&	errorPages() const;
		std::string const&					autoindex() const;
		size_t 								maxBodySize() const;
		std::vector<std::string> const&		authorizedMethods() const;
		bool								isCgi() const;
		bool								isUpload() const;

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
		

		void		_locationDirectives();
		void		_serverDirectives();
};

std::ostream&	operator<<(std::ostream& o, ResponseContext const& rhs);

#endif