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

		Request const&					request(void) const;
		ServerContext const&			serverContext(void) const;
		ResponseContext const&			responseContext(void) const;
		std::string const&				scriptPath(void) const;
		std::vector<char> const&		output(void) const;
		envp_t const&					envpMap(void) const;
		char**							envp(void) const;
		size_t							envSize(void) const;
		std::string const&				cmd(void) const;
		char**							argv(void) const;
		
		void	setScriptPath(std::string const&);
		void	setOutput(std::vector<char> const&);
		void	setEnvpMap(envp_t const&);
		void	setEnvp(char**);
		void	setEnvSize(size_t);
		void	setCmd(void);
		void	setArgv(void);

		void	execute(void);

	private:
		CGI(void);

		Request				_request;
		ResponseContext		_responseContext;
		
		std::string			_scriptPath;
		std::vector<char>	_output;
		envp_t		 		_envpMap;
		char**				_envp;
		size_t				_envSize;
		std::string			_cmd;
		char**				_argv;


		void	_generateEnvp(void);
		void	_generateEnvpMap(void);
		void	_mapToEnvp(void);
		void	_waitChild(pid_t);
};

std::ostream&	operator<<(std::ostream& o, CGI const& rhs);

#endif