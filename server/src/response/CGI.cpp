#include "CGI.hpp"

CGI::CGI() {}

CGI::CGI(std::string const& scriptPath) : _scriptPath(scriptPath) {}

CGI::CGI(CGI const& rhs) {
	*this = rhs;
}

CGI& CGI::operator=(CGI const& rhs) {
	if (this != &rhs) {
		this->_scriptPath = rhs.scriptPath();
		this->_envp = rhs.envp();
		this->_argv = rhs.argv();
	}
	return *this;
}

CGI::~CGI() {}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

std::string const& 				CGI::scriptPath() const { return this->_scriptPath; }
std::vector<std::string> const&	CGI::envp() const { return this->_envp; }
std::vector<std::string> const&	CGI::argv() const { return this->_argv; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	CGI::setScriptPath(std::string const& scriptPath) { this->_scriptPath = scriptPath; }
void	CGI::setEnvp(std::vector<std::string> const& envp) { this->_envp = envp; }
void	CGI::setArgv(std::vector<std::string> const& argv) { this->_argv = argv; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void CGI::execute() {

	int p[2];
	if (pipe(p) == -1) {
		perror("pipe");
		throw HttpStatus("500");
	}

	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		throw HttpStatus("500");
	}	

	if (pid == 0)
	{
		close(p[0]);
		dup2(p[1], STDOUT_FILENO);

		char* argv[] = {(char*)_scriptPath.c_str(), NULL};
		char* envp[] = {const_cast<char*>("QUERY_STRING=value"), NULL};

		if (execve(_scriptPath.c_str(), argv, envp) == -1) {
			perror("execve");
		}
		throw HttpStatus("500");
	}
	else
	{
		close(p[1]);

		int status;
		waitpid(pid, &status, 0);

		char buffer[4096];
		ssize_t bytesRead;
		while ((bytesRead = read(p[0], buffer, sizeof(buffer) - 1)) > 0) {
			buffer[bytesRead] = '\0';
			std::cout << "CGI Output: " << buffer << std::endl;
		}
	}
}