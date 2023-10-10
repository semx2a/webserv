#include "CGI.hpp"

CGI::CGI(std::string const& scriptPath, Request const& req, ResponseContext const& sc) :
																				_request(req),
																				_responseContext(sc),
																				_scriptPath(scriptPath),
																				_output("") {}

CGI::CGI(CGI const& rhs) :
						_request(rhs.request()),
						_responseContext(rhs.responseContext()),
						_scriptPath(rhs.scriptPath()),
						_output(rhs.output()) {
	*this = rhs;
}

CGI& CGI::operator=(CGI const& rhs){
	if (this != &rhs) {

		//TODO
	}
	return *this;
}

CGI::~CGI() {}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

Request const& 			CGI::request() const { return this->_request; }
ResponseContext const& 	CGI::responseContext() const { return this->_responseContext; }
std::string const& 		CGI::scriptPath() const { return this->_scriptPath; }
std::string const& 		CGI::output() const { return this->_output; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	CGI::setScriptPath(std::string const& scriptPath) { this->_scriptPath = scriptPath; }
void	CGI::setOutput(std::string const& output) { this->_output = output; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void	CGI::generateEnvp() {

}

void CGI::execute() {

	std::string cmd; 
	if (_responseContext.cgi() == ".php")
		cmd = strdup("/usr/bin/php-cgi");
	else if (_responseContext.cgi() == ".py")
		cmd = strdup("/usr/bin/python3");
	else {
		std::cout << "CGI: Unknown CGI extension: " << _responseContext.cgi() << std::endl;
		throw HttpStatus("500");
	}

	char** argv = new char*[2];
	argv[0] = strdup(cmd.c_str());
	argv[1] = strdup(_scriptPath.c_str());
	argv[2] = NULL;

	char** envp = new char*[6];
	//std::string query = "QUERY_STRING=" + _scriptPath.c_str();
	envp[0] = strcpy(new char[std::string("PATH_INFO=" + _scriptPath).size() + 1], std::string("PATH_INFO=" + _scriptPath).c_str());
	envp[1] = strcpy(new char[std::string("QUERY_STRING=" + _request.query()).size() + 1], std::string("QUERY_STRING=" + _request.query()).c_str());
	envp[2] = strcpy(new char[std::string("REQUEST_METHOD=" + _request.method()).size() + 1], std::string("REQUEST_METHOD=" + _request.method()).c_str());
	envp[3] = strcpy(new char[std::string("SERVER_PROTOCOL=" + _request.method()).size() + 1], std::string("SERVER_PROTOCOL=" + _request.method()).c_str());
	envp[4] = strcpy(new char[std::string("REDIRECT_STATUS=" + std::string("200")).size() + 1], std::string("REDIRECT_STATUS=" + std::string("200")));
	envp[5] = NULL;

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

		if (execve(cmd.c_str(), argv, envp) == -1) {
			perror("execve");
		}
		delete[] argv;
		delete[] envp;
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
			_output += buffer;
		}
	}
}
