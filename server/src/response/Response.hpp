#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <fstream>

extern "C" {
	#include <sys/types.h>
	#include <dirent.h> //opendir closedir readdir
}

#include "ResponseContext.hpp"

#include "StatusCodes.hpp"
#include "MimeTypes.hpp"

#include "CGI.hpp"

#include "HttpStatus.hpp"
#include "StatusLine.hpp"
#include "Headers.hpp"
#include "Body.hpp"

class Response {

	public:

		Response(Request const&, ResponseContext const&, HttpStatus const&);
		Response(Response const&);
		~Response();
		Response& operator=(Response const&);

		// :::::::::::::::::::::::::::: ACCESSORS
		Request const&			request(void) const;
		ResponseContext const&	responseContext(void) const;
		
		HttpStatus const&		status(void) const;
		Body const&				body(void) const;

		std::string const&		path(void) const;
		std::string const&		boundary(void) const;
		std::string const&		responseStr(void) const;
		std::string const&		customHeaders(void) const;
		std::string const&		filePath(void) const;

		// ::::::::::::::::::::::::::::::: MUTATORS
		void		setRequest(Request const&);
		void		setResponseContext(ResponseContext const&);

		void		setStatus(HttpStatus const&);
		void		setBody(Body const&);

		void		setPath(std::string const&);
		void        setBoundary(std::string const&);
		void		setResponseStr(std::string const&);
		void		setCustomHeaders(std::string const&);
		void 	  	setFilePath(std::string const&);
		void		buildResponse(void);

	private:

		Response(void);

		// ::::::::::::::::::::::::::: ATTRIBUTES
		// CONTEXT
		Request				_request;
		ResponseContext		_responseContext;
		HttpStatus			_status;
		Body				_body;

		// UTILS
		std::string			_path;
		std::string			_boundary;
		std::string			_responseStr;
		std::string			_customHeaders;
		std::string		    _filePath;

		// :::::::::::::::::::::::::: HTTP METHODS
		struct MethodsMap {
			typedef std::map<std::string, void(Response::*)()> type;
		};
		void				_checkAuthorizedMethod(void);
		MethodsMap::type	_initMethods(void);

		// GET
		void				_handleGet(void);
		void				_expandDirectory(void);
		void				_autoIndex(void);
		void				_runCgi(void);

		// POST
		bool 				_bodyBoundary(std::string, std::vector<char> &);
		void				_postData(void);
		void				_handlePost(void);
		void				_handleUpload(void);

		// DELETE
		void				_handleDelete(void);

		// ERROR
		void				_handleError(void);
};

std::ostream& operator<<(std::ostream&, Response const&);

#endif