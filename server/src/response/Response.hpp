#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# define DEBUG_RESPONSE

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <fstream>

extern "C" {
	#include <sys/types.h>
	#include <dirent.h> //opendir closedir readdir
}

#include "Request.hpp"
#include "ServerContext.hpp"

#include "StatusCodes.hpp"
#include "MimeTypes.hpp"

#include "StatusLine.hpp"
#include "Headers.hpp"
#include "Body.hpp"

typedef std::map<std::string, LocationContext>::const_iterator	t_locationIterator;
typedef std::map<std::string, ServerContext>::const_iterator	t_serverIterator;

class Response {

	public:

		Response();
		Response(Request const& request, ServerContext const& serverContext);
		Response(Response const& rhs);
		~Response();
		Response& operator=(Response const& rhs);

		// :::::::::::::::::::::::::::::: ACCESSORS
		// CONTEXT
		Request const&		request() const;

		// COMPONENTS
		std::string const&	statusLine() const;
		std::string const&	headers() const;
		std::string const&	body() const;
		std::string const&	responseStr() const;

		// ::::::::::::::::::::::::::::::: METHODS
		// GET
		void		handleGet();
		void		handlePost();
		void		handleDelete();

		// :::::::::::::::::::::::::::: EXCEPTIONS
		class HttpError : public std::exception {
			private:
				std::string _statusCode;

			public:
				HttpError(std::string const& statusCode);
				virtual ~HttpError() throw();

				std::string const& 		statusCode() const;
				virtual const char*		what() const throw();
		};

	private:

		// ::::::::::::::::::::::::::: ATTRIBUTES
		// CONTEXT
		Request			_request;
		ServerContext	_serverContext;

		// UTILS
		std::string		_path;
		std::string		_contentType;

		// COMPONENTS
		std::string		_statusLine;
		std::string		_headers;
		std::string		_body;

		std::string		_responseStr;

		// :::::::::::::::::::::::::::::: METHODS
		void		_expandTarget();
		void		_setRootOrAlias(t_locationIterator, std::string const&, std::string&);
		// GET
		bool		_isDirectory();
		void		_expandDirectory();
		void		_autoIndex();
		void		_assignIndex(std::vector<std::string> const&);
		bool		_isCgi();
		void		_runCgi();


};

std::ostream& operator<<(std::ostream&, Response const&);

#endif