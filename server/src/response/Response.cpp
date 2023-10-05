#include "Response.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Response::Response() : _status("200") {

	//TODO
}

Response::Response(Request const& request, ServerContext const& serverContext, HttpStatus const& status) : _request(request), _serverContext(serverContext), _status(status) {

	StatusCodes	statusCodes;
	MimeTypes	mimeTypes;

	try {
		
		if (_status.statusCode() == "200" || _status.statusCode() == "202") {	
			
			_checkAllowedMethods();
			_expandTarget();
			std::cout << GREEN << "PATH: " << _path << RESET << std::endl;
			if (_request.method() == "GET")
				handleGet();
			else if (_request.method() == "POST")
				handlePost();
			else if (_request.method() == "DELETE")
				handleDelete();
			else
				throw HttpStatus("405"); // method not allowed

			//_statusLine = "HTTP/1.1 200 OK";
			_statusLine = "HTTP/1.1 " + _status.statusCode() + " " + statusCodes.getReasonPhrase(_status.statusCode());
			_statusLine += CRLF;
		}
		else
			throw HttpStatus(_status.statusCode());
	}
	catch (HttpStatus& e) {
  		
		#ifdef DEBUG_RESPONSE
		std::cout << "HttpStatus: " << e.statusCode() << std::endl;
		#endif

		StatusLine	statusLine(e.statusCode(), statusCodes);
		
		statusLine.build();
		this->_statusLine = statusLine.getMessage();
		
		std::stringstream bodyError;
		bodyError << "<html><body><h1>" << e.statusCode() << "</h1></body></html>";
		_body = bodyError.str();
	}
	std::string ext = _path.substr(_path.find_last_of('.') + 1);
	std::cout << "PATH: " << _path << std::endl;
	std::cout << "EXTENSION: " << ext << std::endl;
	std::cout << "MIME TYPE: " << mimeTypes.getMimeType(ext) << std::endl;
	std::stringstream headers;
	headers << "Content-Type: " << mimeTypes.getMimeType(ext) << CRLF;
	headers << "Content-Length: " << _body.length() << CRLF;
	headers << CRLF;
	_headers = headers.str();
	_responseStr = _statusLine + _headers + _body;
}

Response::Response(Response const& rhs) : _status("200") {

	if (this != &rhs)
		*this = rhs;
}

Response::~Response() {}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMPARISON OPERATORS::

Response& Response::operator=(Response const& rhs)
{
	if (this != &rhs)
	{
		//TODO
	}
	return (*this);
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: GETTERS::

Request const&			Response::request() const { return _request; }
ServerContext const&	Response::serverContext() const { return _serverContext; }

std::string const&		Response::path() const { return _path; }
std::string const&		Response::contentType() const { return _contentType; }

std::string const&		Response::statusLine() const { return _statusLine; }
std::string const&		Response::headers() const { return _headers; }
std::string const&		Response::body() const { return _body; }
std::string const&		Response::responseStr() const { return _responseStr; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: SETTERS::

void	Response::setRequest(Request const& request) { _request = request; }
void	Response::setServerContext(ServerContext const& serverContext) { _serverContext = serverContext; }

void	Response::setPath(std::string const& path) { _path = path; }
void	Response::setContentType(std::string const& contentType) { _contentType = contentType; }

void	Response::setStatusLine(std::string const& statusLine) { _statusLine = statusLine; }
void	Response::setHeaders(std::string const& headers) { _headers = headers; }
void	Response::setBody(std::string const& body) { _body = body; }
void	Response::setResponseStr(std::string const& responseStr) { _responseStr = responseStr; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMMON::

void	Response::_expandTarget() {

	std::string target = _request.target();
	std::string path;

	t_locationIterator it = _serverContext.locations().find(target);
	t_locationIterator itEnd = _serverContext.locations().end();

	if (it != itEnd) {
		_setRootOrAlias(it, target, path);
	}
	if (path.empty()) { // if no root or alias in location or no location at all
		path = _serverContext.root() + target;
	}
	_path = path;

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Expanded path: " << _path << std::endl;
	#endif
}

void	Response::_setRootOrAlias(t_locationIterator it, std::string const& target, std::string& path) {

	std::string root = it->second.root();
	std::string alias = it->second.alias();
	if (!root.empty()) {
		path = root + target;
	}
	else if (!alias.empty()) {
		path = alias;
	}
}

void	Response::_checkAllowedMethods() {

	t_locationIterator it = _serverContext.locations().find(_request.target());
	t_locationIterator itEnd = _serverContext.locations().end();

	if (it != itEnd) {
		std::vector<std::string> const& authorizedMethods = it->second.authorizedMethods();
		if (std::find(authorizedMethods.begin(), authorizedMethods.end(), _request.method()) == authorizedMethods.end()) {
			std::cout << "meow" << std::endl;
			throw HttpStatus("404");
		}
	}
	else if (std::find(_serverContext.authorizedMethods().begin(), _serverContext.authorizedMethods().end(), _request.method()) == _serverContext.authorizedMethods().end()) {
		std::cout << "METHOD: " << _request.method() << std::endl;
			std::cout << "nyah" << std::endl;
		throw HttpStatus("404");
	}
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: HTTP METHODS::

void	Response::handleGet () {

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Entering handleGet" << std::endl;
	#endif

	if (_isDirectory()) {
		_expandDirectory();
	}
	else if (_isCgi()) {
		_runCgi();
	}
	std::ifstream	file(_path.c_str());
	if (!file.is_open()) {
		throw HttpStatus("404");
		return ;
	}

	std::stringstream	bodyContent;
	std::string			line;
	while (std::getline(file, line) && !file.eof())
		bodyContent << line << std::endl;
	file.close();
	_body = bodyContent.str();
}

void	Response::handlePost() {

	
}

void	Response::handleDelete(void) {

	if (access(this->_path.c_str(), F_OK) == -1)
		throw HttpStatus("404");
	else if (access(this->_path.c_str(), W_OK | X_OK) == -1)
		throw HttpStatus("403");
	if (std::remove(this->_path.c_str()) != 0)
		throw HttpStatus("204");
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: EXPANSION::

void	Response::_expandDirectory() {

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Target is a directory" << std::endl;
	#endif

	if (_serverContext.autoindex() == true) {
		_autoIndex();
		return ;
	}
	else {
		t_locationIterator it = _serverContext.locations().find(_request.target());
		t_locationIterator itEnd = _serverContext.locations().end();

		std::string index;
		if (it != itEnd) {
			_assignIndex(it->second.index());
		}
		else if (not _serverContext.index().empty()) {
			_assignIndex(_serverContext.index());
		}
		else {
			throw HttpStatus("404");
		}
	}
}

void	Response::_assignIndex(std::vector<std::string> const& indexVec) {

	for (size_t i = 0; i < indexVec.size(); i++) {
		
		_path += indexVec[i];
		
		#ifdef DEBUG_RESPONSE
		std::cout << "[DEBUG] Trying to open " << _path << std::endl;
		#endif

		std::ifstream	file(_path.c_str());
		if (file.is_open())
			return;
			
		_path = _path.substr(0, _path.size() - indexVec[i].size());
	}
	throw HttpStatus("404");
}

void	Response::_autoIndex() {

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Entering AutoIndex" << std::endl;
	#endif
	
	std::string	dirName(this->path());
	DIR *dir = opendir(this->path().c_str());

	if (dir == NULL)
	{
		std::cerr << "error: could not open [" << this->path() << "]" << std::endl;
		return ;
	}

	std::stringstream page;
	
	page 	<< "<!DOCTYPE html>" << std::endl
			<< "<html>" << std::endl
			<< "<head>" << std::endl
			<< "<title>webserv</title>" <<std::endl
			<< "</head>" << std::endl
			<< "<body>" <<std::endl
			<< "<h1>Index of ";

	if (dirName[0] != '/')
		dirName = "/" + dirName;

	page	<< this->path()
			<< "</h1>" << std::endl
			<< "<p>" << std::endl;
	
	for (struct dirent *dir_entry = readdir(dir); dir_entry; dir_entry = readdir(dir)) {
		
		page << this->_get_link(std::string(dir_entry->d_name), this->path());
	}

	page 	<< "</body>" << std::endl
			<< "</html>" << std::endl;
	
	closedir(dir);
	this->setBody(page.str());

}

void	Response::_runCgi() {

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Entering runCgi" << std::endl;
	#endif

	
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: UTILS::

std::string		Response::_get_link(std::string const &dir_entry, std::string const &route)
{
	std::stringstream   link;

	if (*route.rbegin() == '/')
		link << "\t\t<p><a href=\"" + route + dir_entry + "\">" + dir_entry + "</a></p>\n";
	else
		link << "\t\t<p><a href=\"" << route << "/" + dir_entry + "\">" + dir_entry + "</a></p>\n";
	return link.str();
}

bool	Response::_isDirectory() {

	return _path.find_last_of('/') == _path.size() - 1;
}

bool	Response::_isCgi() {

	return _path.find(".py") != std::string::npos;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::OUTPUT OPERATOR OVERLOAD::

std::ostream& operator<<(std::ostream& o, Response const& rhs) {
	
	o << rhs.responseStr();
	return (o);
}
