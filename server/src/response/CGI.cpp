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
size_t					CGI::envSize() const { return this->_envSize; }
std::string const&		CGI::cmd() const { return this->_cmd; }
char**					CGI::argv() const { return this->_argv; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	CGI::setScriptPath(std::string const& scriptPath) { this->_scriptPath = scriptPath; }
void	CGI::setOutput(std::string const& output) { this->_output = output; }
void	CGI::setEnvpMap(envp_t const& envpMap) { this->_envpMap = envpMap; }
void	CGI::setEnvp(char** envp) { this->_envp = envp; }
void	CGI::setEnvSize(size_t envSize) { this->_envSize = envSize; }
void	CGI::setCmd() {

	if (this->_scriptPath.find(".php"))
		this->_cmd = "/usr/bin/php-cgi";
	else if (this->_scriptPath.find(".py"))
		this->_cmd = "/usr/bin/python3";
	else {
		std::cout << "CGI: Unknown CGI extension" << std::endl;
		throw HttpStatus("500");
	}
}
void	CGI::setArgv() {

	this->_argv = new char*[3];
	if (this->_argv == NULL)
		throw HttpStatus("500");

	this->_argv[0] = new char[this->_cmd.length() + 1];
	this->_argv[0] = strdup(this->_cmd.c_str());
	this->_argv[1] = new char[this->_scriptPath.length() + 1];
	this->_argv[1] = strdup(this->_scriptPath.c_str());
	this->_argv[2] = NULL;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void CGI::_mapToEnvp() {

	std::vector<std::string> v;
	for (std::map<std::string, std::string>::const_iterator it = this->_envpMap.begin() ; it != this->_envpMap.end() ; it++)
		v.push_back(it->first + "=" + it->second);

	_envSize = v.size();
	this->_envp = new char*[_envSize + 1];
	std::memset(this->_envp, 0, sizeof(char*) * (_envSize + 1));
	if (this->_envp == NULL)
		throw HttpStatus("500");
	this->_envp[this->_envSize] = NULL;
	
	std::vector<std::string>::size_type index = 0;
	for (std::vector<std::string>::iterator it = v.begin() ; it != v.end() ; ++it)
	{
		this->_envp[index] = new char[it->length() + 1];
		std::strcpy(this->_envp[index], it->c_str());
		if (this->_envp[index] == NULL)
			throw HttpStatus("500");
		index++;
	}
	
	v.erase(v.begin(), v.end());
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

	this->_generateEnvpMap();
	this->_mapToEnvp(); //mallocd

	if (pid == 0)
	{
		close(p[0]);
		dup2(p[1], STDOUT_FILENO);

		this->setCmd();
		this->setArgv(); //mallocd

		#ifdef DEBUG_CGI
		std::cerr << "[DEBUG] CGI: " << *this << std::endl;
		#endif
		
		if (execve(this->cmd().c_str(), this->argv(), this->_envp) == -1) {
			perror("execve");
		}

		for (int i = 0 ; this->_envp[i] != NULL ; i++)
			delete this->_envp[i];
		delete this->_envp;
		for (int i = 0 ; this->_argv[i] != NULL ; i++)
			delete this->_argv[i];
		delete[] this->_argv;
		
		throw HttpStatus("500");
		
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		status = WEXITSTATUS(status);
		if (status != 0)
			throw HttpStatus("500");
		close(p[1]);

		char buffer[4096];
		ssize_t bytesRead;
		while ((bytesRead = read(p[0], buffer, sizeof(buffer) - 1)) > 0) {
			buffer[bytesRead] = '\0';
			_output += buffer;
		}
		if (bytesRead < 0)
			throw HttpStatus("500");
		close(p[0]);

	}
	for (int i = 0 ; this->_envp[i] != NULL ; i++)
		delete this->_envp[i];
	delete this->_envp;

}

// ::::::::::::::::::::::::::::::::::::::::::::::::::OUTPUT OPERATTOR OVERLOAD::

std::ostream& operator<<(std::ostream& o, CGI const& rhs) {

	o << "CGI: " << std::endl;
	o << "\t" << "Script path: " 	<< rhs.scriptPath()		<< std::endl;
	o << "\t" << "Output: " 		<< rhs.output()			<< std::endl;
	o << "\t" << "Cmd: " 			<< rhs.cmd()			<< std::endl;
	o << "\t" << "Argv: " 			<< utl::printCharArray(rhs.argv(), 2) << std::endl;
	//o << "\t" << "EnvpMap: "		<< utl::print_map(rhs.envpMap()) << std::endl;
	//o << "\t" << "Envp: " 			<< utl::printCharArray(rhs.envp(), rhs.envSize()) << std::endl;

	return o;
}