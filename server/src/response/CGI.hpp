#ifndef CGI_HPP
#define CGI_HPP
	
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <iostream>

extern "C" {
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/wait.h>
	#include <fcntl.h>
}

#include "Request.hpp"
#include "ServerContext.hpp"

#include "HttpStatus.hpp"

class CGI {

	public:
		CGI(std::string const&, Request const&, ServerContext const&);
		CGI(CGI const&);
		CGI& operator=(CGI const&);
		~CGI();

		std::string const&		scriptPath() const;
		std::string const&		output() const;

		void	setScriptPath(std::string const&);
		void	setOutput(std::string const&);

		void	execute();

	private:
		CGI();

		Request				_request;
		ServerContext		_serverContext;
		std::string			_scriptPath;
		std::string			_output;
};

#endif