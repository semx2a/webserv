#ifndef CGI_HPP
#define CGI_HPP
	
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

extern "C" {
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/wait.h>
	#include <fcntl.h>
}

#include "HttpStatus.hpp"

class CGI {

	public:
		CGI();
		CGI(std::string const& scriptPath);
		CGI(CGI const& rhs);
		CGI& operator=(CGI const& rhs);
		~CGI();

		std::string const&				scriptPath() const;
		std::vector<std::string> const&	envp() const;
		std::vector<std::string> const&	argv() const;

		void	setScriptPath(std::string const& scriptPath);
		void	setEnvp(std::vector<std::string> const& envp);
		void	setArgv(std::vector<std::string> const& argv);

		void	execute();

	private:
		std::string					_scriptPath;
		std::vector<std::string>	_envp;
		std::vector<std::string>	_argv;
	
};

#endif