#ifndef CGI_HPP
#define CGI_HPP

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <sstream>

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

		Request const&					request() const;
		ServerContext const&			serverContext() const;
		ResponseContext const&			responseContext() const;
		std::string const&				scriptPath() const;
		std::vector<char> const&		output() const;
		envp_t const&					envpMap() const;
		char**							envp() const;
		size_t							envSize() const;
		std::string const&				cmd() const;
		char**							argv() const;
		
		void	setScriptPath(std::string const&);
		void	setOutput(std::vector<char> const&);
		void	setEnvpMap(envp_t const&);
		void	setEnvp(char**);
		void	setEnvSize(size_t);
		void	setCmd();
		void	setArgv();

		void	execute();

	private:
		CGI();

		Request				_request;
		ResponseContext		_responseContext;
		
		std::string			_scriptPath;
		std::vector<char>	_output;
		envp_t		 		_envpMap;
		char**				_envp;
		size_t				_envSize;
		std::string			_cmd;
		char**				_argv;


		void	_generateEnvp();
		void	_generateEnvpMap();
		void	_mapToEnvp();
		void	_waitChild(pid_t);
};

std::ostream&	operator<<(std::ostream& o, CGI const& rhs);

#endif