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
#include "HttpStatus.hpp"

typedef std::map<std::string, LocationContext>::const_iterator	t_locationIterator;
typedef std::map<std::string, ServerContext>::const_iterator	t_serverIterator;

class Response {

	public:

		Response();
		Response(Request const&, ServerContext const&t, HttpStatus const&);
		Response(Response const&);
		~Response();
		Response& operator=(Response const&);

		// :::::::::::::::::::::::::::: ACCESSORS
		// CONTEXT
		Request const&			request() const;
		ServerContext const&	serverContext() const;
		HttpStatus const&		status() const;

		// UTILS
		std::string const&		path() const;
		std::string const&		contentType() const;

		// COMPONENTS
		std::string const&		statusLine() const;
		std::string const&		headers() const;
		std::string const&		body() const;
		std::string const&		responseStr() const;

		// ::::::::::::::::::::::::::::::: MUTATORS

		// CONTEXT
		void		setRequest(Request const&);
		void		setServerContext(ServerContext const&);
		void		setStatus(HttpStatus const&);

		// UTILS
		void		setPath(std::string const&);
		void		setContentType(std::string const&);

		// COMPONENTS
		void		setStatusLine(std::string const&);
		void		setHeaders(std::string const&);
		void		setBody(std::string const&);
		void		setResponseStr(std::string const&);

		// :::::::::::::::::::::::::: HTPP METHODS
		void		handleGet();
		void		handlePost();
		void		handleDelete();

		struct MethodsMap {
			typedef std::map<std::string, void(Response::*)()> type;
		};
		

	private:

		// ::::::::::::::::::::::::::: ATTRIBUTES
		// CONTEXT
		Request			_request;
		ServerContext	_serverContext;
		HttpStatus		_status;

		// UTILS
		std::string		_path;
		std::string		_contentType;

		// COMPONENTS
		std::string		_statusLine;
		std::string		_headers;
		std::string		_body;

		std::string		_responseStr;

		// :::::::::::::::::::::::::::::: METHODS
		MethodsMap::type 	_initMethods();
		void				_checkAllowedMethods();
		void				_expandTarget();
		void				_setRootOrAlias(t_locationIterator, std::string const&, std::string&);
		// GET
		void				_expandDirectory();
		void				_autoIndex();
		void				_assignIndex(std::vector<std::string> const&);
		void				_runCgi();

		// ::::::::::::::::::::::::::::::: UTILS
		std::string			_get_link(std::string const &, std::string const &);
		bool				_isDirectory();
		bool				_isCgi();


};

std::ostream& operator<<(std::ostream&, Response const&);

#endif