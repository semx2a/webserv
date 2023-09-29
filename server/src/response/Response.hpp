#ifndef RESPONSE_HPP
# define RESPONSE_HPP

//# define DEBUG_RESPONSE

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <fstream>

# include "macros.hpp"
# include "Request.hpp"
# include "ServerContext.hpp"

extern "C" {
	#include <sys/types.h>
	#include <dirent.h> //opendir closedir readdir
}

typedef std::map<std::string, std::string>						t_lexicon;
typedef std::map<std::string, LocationContext>::const_iterator	t_locationIterator;
typedef std::map<std::string, ServerContext>::const_iterator	t_serverIterator;

class Response {

	public:

		Response();
		Response(Response const& rhs);
		Response(Request const&, ServerContext const&);
		~Response();
		Response& operator=(Response const& rhs);

		Request const&			getRequest() const;
		ServerContext const&	getServerContext() const;
		t_lexicon				getStatusMessages() const;
		t_lexicon				getMimeTypes() const;
		bool					getCgi() const;
		std::string const&		getStatusMessage(std::string const &) const;
		std::string const&		getMimeType(std::string const&) const;
		std::string const&		getStatusCode() const;
		std::string const&		getVersion() const;
		std::string const&		getStatusLine() const;
		std::string const&		getHeaders(void) const;
		std::string const&		getBodyContent() const;
		std::string const&		getResponse() const;
		std::string const&		getTargetFinalPath() const;

		void	setStatusMessages(std::string const&);
		void	setStatusCode(std::string const&);
		void	setVersion(std::string const&);
		void	setMimeTypes(std::string const&);
		void	setStatusLine(std::string const&);
		void	setBodyContent(std::string const&);
		void	setHeaders(std::string const&);
		void	setResponse(std::string const&);
		void	setCgi(bool const&);
		void	setTargetFinalPath(std::string const&);
		
		void	buildResponse();

	private:

		Request			_request;
		ServerContext	_serverContext;

		t_lexicon		_statusMessages;
		t_lexicon 		_mimeTypes;
		
		std::string		_targetFinalPath;
		bool			_isCGI;

		std::string		_statusCode;
		std::string		_version;

		std::string		_statusLine;
		std::string		_headers;
		std::string		_bodyContent;

		std::string		_response;

		std::string _trim(std::string const&);
		t_lexicon	_initFile(std::string const&);


		void		_buildStatusLine();
		void		_buildHeaders();
		void		_buildBody();
};

std::ostream& operator<<(std::ostream& o, Response const& rhs);

#endif