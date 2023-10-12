#include "CGI.hpp"

CGI::CGI(std::string const& scriptPath, Request const& req, ResponseContext const& sc) :	_request(req),
																							_responseContext(sc),
																							_scriptPath(scriptPath),
																							_output() {}

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
std::vector<char> const& 		CGI::output() const { return this->_output; }
envp_t const&			CGI::envpMap() const { return this->_envpMap; }
char**					CGI::envp() const { return this->_envp; }
size_t					CGI::envSize() const { return this->_envSize; }
std::string const&		CGI::cmd() const { return this->_cmd; }
char**					CGI::argv() const { return this->_argv; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	CGI::setScriptPath(std::string const& scriptPath) { this->_scriptPath = scriptPath; }
void	CGI::setOutput(std::vector<char> const& output) { this->_output = output; }
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

void CGI::execute() {

/* 	int p[2];
	if (pipe(p) == -1) {
		perror("pipe");
		throw HttpStatus("500");
	}

	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		throw HttpStatus("500");
	}	

	// CHILD PROCESS
	if (pid == 0) {

		close(p[0]);
		if (dup2(p[1], STDOUT_FILENO) == -1) {
			perror("dup2");
			close(p[1]);
			throw HttpStatus("500");
		}
		close(p[1]);

		this->_generateEnvp();
		this->setCmd();
		this->setArgv();

		#ifdef DEBUG_CGI
		std::cerr << "[DEBUG] " << *this << std::endl;
		#endif

		execve(this->cmd().c_str(), this->argv(), this->_envp);
		perror("execve");

		utl::deleteCharArray(this->_argv);
		utl::deleteCharArray(this->_envp);
		exit (EXIT_FAILURE);
	}

	// PARENT PROCESS
	else {

		close(p[1]);

		char buffer[4096];
		ssize_t bytesRead;
		bytesRead = read(p[0], buffer, sizeof(buffer) - 1);
		buffer[bytesRead] = '\0';
		std::cout << "[DEBUG] Buffer: " << buffer << std::endl;
		_output += buffer;

		if (bytesRead < 0)
			throw HttpStatus("500");
		
		pid_t	wpid;
		int		status;
		wpid = wait(&status);
		if (pid != 0)
			throw HttpStatus("500");
	} */
		int pid, stat, fd[2];

	if (access(_responseContext.path().c_str(), X_OK) == -1)
		throw HttpStatus("403");
	if (pipe(fd) != 0)
		throw HttpStatus("503");
	pid = fork();
	if (pid == -1)
		throw HttpStatus("500");
	if (pid == 0) // child process
	{
		close(fd[0]);
		close(STDOUT_FILENO);
		close(STDIN_FILENO);
	
		std::vector<char> copy(_request.body());
		copy.push_back('\0');
		std::FILE* tmpf = std::tmpfile();
		std::fputs(&copy[0], tmpf);
		std::rewind(tmpf);
		if (dup2(fileno(tmpf), STDIN_FILENO) == -1)
			exit(1);


		if (dup2(fd[1], STDOUT_FILENO) == -1)
			exit(1);
		close(fd[1]);
		
		//char* binPath = strdup(getProgName(path).c_str());
		//char* progPath = strdup(path.c_str());
		//char* argv[3] = { binPath, progPath, NULL };
		this->setArgv();
		this->setCmd();
		this->_generateEnvp();

		execve(_cmd.c_str(), _argv, _envp);
		utl::deleteCharArray(this->_argv);
		utl::deleteCharArray(this->_envp);
		exit(1);
	}
	else // parent process
	{
		//utl::deleteCharArray(this->_envp);
		waitpid(pid, &stat, 0);
		stat = WEXITSTATUS(stat);
		if (stat != 0)
			throw HttpStatus("500");
		close(fd[1]);
		int	 ret = 0;
		std::vector<char> buff(1024, 0);
		while ((ret = read(fd[0], &buff[0], 1024)) > 0)
			_output.insert(_output.end(), buff.begin(), buff.begin() + ret);
		if (ret < 0)
			throw HttpStatus("500");
		close(fd[0]);
	}
}

void	CGI::_generateEnvp() {

	this->_generateEnvpMap();
	this->_mapToEnvp();
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
	this->_envpMap["PATH_TRANSLATED"] =  this->scriptPath();
	this->_envpMap["SCRIPT_FILENAME"] = this->scriptPath();
	this->_envpMap["SCRIPT_NAME"] = this->scriptPath();
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

// ::::::::::::::::::::::::::::::::::::::::::::::::::OUTPUT OPERATTOR OVERLOAD::

std::ostream& operator<<(std::ostream& o, CGI const& rhs) {

	o << "CGI: " << std::endl;
	o << "\t" << "Script path: " 	<< rhs.scriptPath()		<< std::endl;
	o << "\t" << "Output: " 		<< utl::print_vector_of_char(rhs.output())			<< std::endl;
	o << "\t" << "Cmd: " 			<< rhs.cmd()			<< std::endl;
	o << "\t" << "Argv: " 			<< utl::printCharArray(rhs.argv(), 2) << std::endl;
//	o << "\t" << "EnvpMap: "		<< utl::print_map(rhs.envpMap()) << std::endl;
	o << "\t" << "Envp: " 			<< utl::printCharArray(rhs.envp(), rhs.envSize()) << std::endl;

	return o;
}