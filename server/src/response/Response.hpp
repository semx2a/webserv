#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# define DEBUG_RESPONSE

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <fstream>

# include "macros.hpp"
# include "Request.hpp"
# include "ServerContext.hpp"
# include "ResponseHandler.hpp"
# include "ResponseBuilder.hpp"
# include "StatusCodes.hpp"
# include "MimeTypes.hpp"

extern "C" {
	#include <sys/types.h>
	#include <dirent.h> //opendir closedir readdir
}

class ResponseHandler;
class ResponseBuilder;

class Response {

	public:

		//Response();
		Response(Response const& rhs);
		Response(Request const&, ServerContext const&);
		~Response();
		Response& operator=(Response const& rhs);

		Request const&			getRequest() const;
		ServerContext const&	getServerContext() const;

		StatusCodes const&		getStatusCodes() const;
		MimeTypes const&		getMimeTypes() const;

		std::string const&		getPath() const;

		std::string const&		getStatusLine() const;
		std::string const&		getHeaders(void) const;
		std::string const&		getBody() const;

		std::string const&		getResponseStr() const;

		void	setRequest(Request const&);
		void	setServerContext(ServerContext const&);

		void	setStatusCodes(StatusCodes const&);
		void	setMimeTypes(MimeTypes const&);

		void	setPath(std::string const&);

		void	setStatusLine(std::string const&);
		void	setHeaders(std::string const&);
		void	setBody(std::string const&);
		void	setResponseStr(std::string const&);
		
	private:

		Request			_request;
		ServerContext	_serverContext;
//		AHandler *		_method;
		StatusCodes		_statusCodes;
		MimeTypes 		_mimeTypes;
		
		std::string		_path;

		std::string		_statusLine;
		std::string		_headers;
		std::string		_body;
		std::string		_responseStr;

};

std::ostream& operator<<(std::ostream& o, Response const& rhs);

#endif