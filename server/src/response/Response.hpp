#ifndef RESPONSE_HPP
# define RESPONSE_HPP

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

		t_lexicon				getStatusCodes() const;
		t_lexicon				getMimeTypes() const;
		std::string const&		getStatusCode(std::string const &) const;
		std::string const&		getMimeType(std::string const&) const;
		std::string const&		getStatusLine() const;
		std::string const&		getVersion() const;
		std::string const&		getResponse() const;
		ServerContext const&	getServerContext() const;
		Request const&			getRequest() const;

		void	setStatusCodes(std::string const&);
		void	setMimeTypes(std::string const&);
		void	setStatusLine(std::string const&);
		void	setVersion(std::string const&);
		void	setResponse(std::string const&);
		
		void	handleResponse();
		void	buildResponse();

	private:

		Request			_request;
		ServerContext	_serverContext;

		t_lexicon		_statusCodes;
		t_lexicon 		_mimeTypes;

		std::string		_statusLine;
		std::string		_version;
		std::string		_response;

		std::string		_targetFinalPath;
		bool			_isCGI;
		std::string		_bodyContent;


		std::string _trim(std::string const&);
		t_lexicon	_initFile(std::string const&);
		void		_checkTarget();
		void 		_handleAutoIndex();
		void		_handleGet();
		void		_handlePost();
		void		_handleDelete();
		void		_assignIndex(std::vector<std::string> const&);
};

std::ostream& operator<<(std::ostream& o, Response const& rhs);

#endif