#ifndef AHANDLER_HPP
# define AHANDLER_HPP

# define DEBUG_RESPONSE

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <fstream>

# include "macros.hpp"
# include "Request.hpp"
# include "ServerContext.hpp"
# include "StatusCodes.hpp"
# include "MimeTypes.hpp"

extern "C" {
	#include <sys/types.h>
	#include <dirent.h> //opendir closedir readdir
}
# define DEBUG_RESPONSE_HANDLER

#include "Response.hpp"

typedef std::map<std::string, LocationContext>::const_iterator	t_locationIterator;
typedef std::map<std::string, ServerContext>::const_iterator	t_serverIterator;

class AHandler {

	public:
		
		AHandler(Request const&, ServerContext const&);
		AHandler(AHandler const&);
		virtual ~AHandler();
		AHandler& operator=(AHandler const&);

		Request const&			getRequest() const;
		ServerContext const&	getServerContext() const;
		StatusCodes const&		getStatusCodes() const;
		MimeTypes const&		getMimeTypes() const;
		std::string const&		getPath() const;

		void	setRequest(Request const&);
		void	setServerContext(ServerContext const&);
		void	setStatusCodes(StatusCodes const&);
		void	setMimeTypes(MimeTypes const&);
		void	setPath(std::string const&);

		virtual void	handle() = 0;

	protected:
		AHandler();

		Request			_request;
		ServerContext	_serverContext;
		StatusCodes		_statusCodes;
		MimeTypes 		_mimeTypes;
		std::string		_path;

		void		_setPath();

};

#endif