#ifndef CGI_HPP
#define CGI_HPP
	
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <cstring>
extern "C" {
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/wait.h>
	#include <fcntl.h>
}

#include "ResponseContext.hpp"
#include "Request.hpp"

#include "HttpStatus.hpp"


typedef std::map<std::string, std::string> envp_t;

class CGI {

	public:
		CGI(std::string const&, Request const&, ResponseContext const&);
		CGI(CGI const&);
		CGI& operator=(CGI const&);
		~CGI();

		Request const&			request() const;
		ServerContext const&	serverContext() const;
		ResponseContext const&	responseContext() const;
		std::string const&		scriptPath() const;
		std::string const&		output() const;
		envp_t const&			envpMap() const;
		char**					envp() const;

		void	setScriptPath(std::string const&);
		void	setOutput(std::string const&);
		void	setEnvpMap(envp_t const&);
		void	setEnvp(char**);

		void	execute();

	private:
		CGI();

		Request			_request;
		ResponseContext	_responseContext;
		
		std::string		_scriptPath;
		std::string		_output;
		envp_t		 	_envpMap;
		char**			_envp;


		void	_generateEnvpMap();
		void	_mapToEnvp();
};

std::ostream&	operator<<(std::ostream& o, CGI const& rhs);

#endif