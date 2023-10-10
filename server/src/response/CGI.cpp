#include "CGI.hpp"

CGI::CGI(std::string const& scriptPath, Request const& req, ResponseContext const& sc) :	_request(req),
																							_responseContext(sc),
																							_scriptPath(scriptPath),
																							_output("") {}

CGI::CGI(CGI const& rhs) :	_request(rhs.request()),
							_responseContext(rhs.responseContext()),
							_scriptPath(rhs.scriptPath()),
							_output(rhs.output()) {
	*this = rhs;
}

CGI& CGI::operator=(CGI const& rhs){
	
	if (this != &rhs) {

		this->_request = rhs.request();
		this->_responseContext = rhs.responseContext();
		this->_scriptPath = rhs.scriptPath();
		this->_output = rhs.output();
	}
	return *this;
}

CGI::~CGI() {}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

Request const& 			CGI::request() const { return this->_responseContext.request(); }
ServerContext const& 	CGI::serverContext() const { return this->_responseContext.serverContext(); }
ResponseContext const& 	CGI::responseContext() const { return this->_responseContext; }
std::string const& 		CGI::scriptPath() const { return this->_scriptPath; }
std::string const& 		CGI::output() const { return this->_output; }
envp_t const&			CGI::envpMap() const { return this->_envpMap; }
char**					CGI::envp() const { return this->_envp; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	CGI::setScriptPath(std::string const& scriptPath) { this->_scriptPath = scriptPath; }
void	CGI::setOutput(std::string const& output) { this->_output = output; }
void	CGI::setEnvpMap(envp_t const& envpMap) { this->_envpMap = envpMap; }
void	CGI::setEnvp(char** envp) { this->_envp = envp; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void CGI::_mapToEnvp() {

	std::vector<std::string> v;
	for (std::map<std::string, std::string>::const_iterator it = this->_envpMap.begin() ; it != this->_envpMap.end() ; it++)
		v.push_back(it->first + "=" + it->second);
	
	char **arr = new char*[v.size() + 1];
	if (arr == NULL)
		throw HttpStatus("500");
	
	std::vector<std::string>::size_type index = 0;
	for (std::vector<std::string>::iterator it = v.begin() ; it != v.end() ; it++)
	{
		std::strcpy(arr[index], it->c_str());
		if (arr[index] == NULL)
			throw HttpStatus("500");
		index++;
	}
	
	arr[index] = NULL;
	this->_envp = arr;
}

void	CGI::_generateEnvpMap() {
	
	// SERVER VARIABLES
	this->_envpMap["SERVER_SOFTWARE"] = "neoserv/1.0";
	this->_envpMap["SERVER_NAME"] = this->serverContext().serverNames()[0];
	this->_envpMap["GATEWAY_INTERFACE"] = "CGI/1.1";

	// REQUEST DEFINED VARIABLES
	this->_envpMap["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_envpMap["SERVER_PORT"] = this->serverContext().port();
	this->_envpMap["REQUEST_METHOD"] = this->request().method();
	this->_envpMap["REDIRECT_STATUS"] = "200";
	this->_envpMap["PATH_INFO"] = this->scriptPath();
	this->_envpMap["PATH_TRANSLATED"] = this->scriptPath();
	this->_envpMap["SCRIPT_NAME"] = this->scriptPath();
	this->_envpMap["SCRIPT_FILENAME"] = this->scriptPath();
	this->_envpMap["QUERY_STRING"] = this->request().query();
	this->_envpMap["REMOTE_HOST"] = "";
	this->_envpMap["REMOTE_ADDR"] = "";
	this->_envpMap["AUTH_TYPE"] = "";
	this->_envpMap["REMOTE_USER"] = "";

	if (this->request().headers().find("content-type") != this->request().headers().end())
		this->_envpMap["CONTENT_TYPE"] = this->request().headers().find("content-type")->second;
	else
		this->_envpMap["CONTENT_TYPE"] = "application/x-www-form-urlencoded";

	this->_envpMap["CONTENT_LENGTH"] = utl::numberToString(this->request().body().size());

	// CLIENT VARIABLES
	this->_envpMap["HTTP_ACCEPT"] = "*/*";
	this->_envpMap["HTTP_ACCEPT_LANGUAGE"] = "en-US,en";
	if (this->request().headers().find("user-agent") != this->request().headers().end())
		this->_envpMap["HTTP_USER_AGENT"] = this->request().headers().find("user-agent")->second;
	if (this->request().headers().find("cookie") != this->request().headers().end())
		this->_envpMap["HTTP_COOKIE"] = this->request().headers().find("cookie")->second;
	this->_envpMap["HTTP_REFERER"] = "";
}

void CGI::execute() {

	std::string cmd; 
	
	if (_scriptPath.find(".php"))
		cmd = strdup("/usr/bin/php-cgi");
	else if (_scriptPath.find(".py"))
		cmd = strdup("/usr/bin/python3");
	else {
		std::cout << "CGI: Unknown CGI extension" << std::endl;
		throw HttpStatus("500");
	}

	char** argv = new char*[2];
	argv[0] = strdup(cmd.c_str());
	argv[1] = strdup(_scriptPath.c_str());
	argv[2] = NULL;

	_generateEnvpMap();
	std::cout << "[DEBUG]: " << *this << std::endl;
	
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
		this->_mapToEnvp();

		if (execve(cmd.c_str(), argv, this->_envp) == -1) {
			perror("execve");
		}
		delete[] argv;
		delete[] this->_envp;
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

// ::::::::::::::::::::::::::::::::::::::::::::::::::OUTPUT OPERATTOR OVERLOAD::

std::ostream& operator<<(std::ostream& o, CGI const& rhs) {

	o << "CGI: " << std::endl;
	o << "\t" << "Script path: " 	<< rhs.scriptPath()		<< std::endl;
	o << "\t" << "Output: " 		<< rhs.output()			<< std::endl;
	o << "\t" << "Envp: " 			<< utl::print_map(rhs.envpMap()) << std::endl;

	return o;
}