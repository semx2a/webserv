#include "Response.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Response::Response(Request const& request, ResponseContext const& responseContext, HttpStatus const& status) : 
														_request(request), 
														_responseContext(responseContext), 
														_status(status),
														_path(responseContext.path()) {

	this->buildResponse();
}

Response::Response(Response const& rhs) : 
								_request(rhs.request()), 
								_responseContext(rhs.responseContext()),
								_status(rhs.status()) {

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
ResponseContext const&	Response::responseContext() const { return _responseContext; }
Body const&				Response::body() const { return _body; }
HttpStatus const&		Response::status() const { return _status; }

std::string const&		Response::path() const { return _path; }
//std::string const&		Response::contentType() const { return _contentType; }
std::string const&		Response::responseStr() const { return _responseStr; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: SETTERS::

void	Response::setRequest(Request const& request) { _request = request; }
void	Response::setResponseContext(ResponseContext const& responseContext) { _responseContext = responseContext; }
void	Response::setBody(Body const& body) { _body = body; }
void	Response::setStatus(HttpStatus const& status) { _status = status; }

void	Response::setPath(std::string const& path) { _path = path; }
//void	Response::setContentType(std::string const& contentType) { _contentType = contentType; }
void	Response::setResponseStr(std::string const& responseStr) { _responseStr = responseStr; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ENTRYPOINT::

void	Response::buildResponse() {

	StatusCodes					statusCodes;
	MimeTypes					mimeTypes;
	Response::MethodsMap::type	methodsMap = _initMethods();
 
	try {
        if (_status.statusCode().find_first_of("45") == 0) {
            
            (this->*(&Response::_handleError))();
            throw HttpStatus(_status.statusCode());
        }
		_checkAuthorizedMethod();
		if (methodsMap.find(_request.method()) == methodsMap.end()) {
			throw HttpStatus("501");
		}
		(this->*methodsMap[_request.method()])();
		_status.setStatusCode("200");
	}
	catch (HttpStatus& e) {
		_status.setStatusCode(e.statusCode());
		_handleError();
	}

 	StatusLine	statusLine(_status.statusCode(), statusCodes.getReasonPhrase(_status.statusCode()));
	Body		body(_body.getContent());
	Headers		headers(mimeTypes.getMimeType(_extension), body.getContentLength(), _responseContext);

	_responseStr = statusLine.getContent() + headers.getContent() + body.getContent();
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: HTTP METHODS::

Response::MethodsMap::type	Response::_initMethods() {
	
	Response::MethodsMap::type	methodsMap;

	methodsMap["GET"] = &Response::_handleGet;
	methodsMap["POST"] = &Response::_handlePost;
	methodsMap["DELETE"] = &Response::_handleDelete;
	methodsMap["ERROR"] = &Response::_handleError;
	
	return methodsMap;
}

void	Response::_handleGet () {

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Entering handleGet" << std::endl;
	#endif

	if (_isDirectory()) {
		_expandDirectory();
	}
	if (not _responseContext.cgi().empty()) {
		_runCgi();
		return ;
	}
	std::ifstream	file(_path.c_str());
	if (!file.is_open()) {
		throw HttpStatus("404");
	}
	_fillBodyWithFileContent(file);
}

void	Response::_fillBodyWithFileContent(std::ifstream& file) {

	std::stringstream	bodyContent;
	std::string			line;
	
	while (std::getline(file, line) && !file.eof()) {
		bodyContent << line << std::endl;
	}
	file.close();
	this->_findExtension();
	this->_body.build(bodyContent.str());
}

void	Response::_handlePost() {

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Entering handlePost" << std::endl;
	#endif
}

void	Response::_handleDelete() {

	if (access(this->_path.c_str(), F_OK) == -1)
		throw HttpStatus("404");
	else if (access(this->_path.c_str(), W_OK | X_OK) == -1)
		throw HttpStatus("403");
	if (std::remove(this->_path.c_str()) != 0)
		throw HttpStatus("204");
}

void	Response::_handleError() {

	int statusCode = std::atoi(_status.statusCode().c_str());
	if (_responseContext.errorPages().find(statusCode) != _responseContext.errorPages().end()) {

		_path = _responseContext.root() + _responseContext.errorPages().find(statusCode)->second;

		#ifdef DEBUG_RESPONSE
		std::cout << "[DEBUG ERROR] path: " << _path << std::endl;
		#endif

		std::ifstream	file(_path.c_str());
		if (file.is_open()) {
			_fillBodyWithFileContent(file);
			return ;
		}
	}
	std::stringstream bodyContent;
	bodyContent	<< "<html><body><h1>" 
				<< _status.statusCode() 
				<< "</h1></body></html>";
	this->_body.build(bodyContent.str());
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: EXPANSION::

void	Response::_checkAuthorizedMethod() {

	std::vector<std::string>::const_iterator it = std::find(_responseContext.authorizedMethods().begin(), _responseContext.authorizedMethods().end(), _request.method());
	if (it == _responseContext.authorizedMethods().end()) {
		this->_status.setStatusCode("405");
	}
}

void	Response::_expandDirectory() {

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Target is a directory" << std::endl;
	#endif

	if (_responseContext.autoindex() == "true") {
		_autoIndex();
		return ;
	}
	for (size_t i = 0; i < _responseContext.index().size(); i++) {
			
		_path += _responseContext.index()[i];
		
		#ifdef DEBUG_RESPONSE
		std::cout << "[DEBUG] Trying to open " << _path << std::endl;
		#endif

		std::ifstream	file(_path.c_str());
		if (file.is_open())
			return;
			
		_path = _path.substr(0, _path.size() - _responseContext.index()[i].size());
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

	CGI	cgi(_path, _request, _responseContext);
	cgi.execute();
	_extension = "py";
	_body.build(cgi.output());
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

void	Response::_findExtension() {

	std::cout << "path: " << _path << std::endl;
	_extension = _path.substr(_path.find_last_of('.') + 1, _path.size() - _path.find_last_of('.') - 1);

	std::cout << "extension: " << _extension << std::endl;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::OUTPUT OPERATOR OVERLOAD::

std::ostream& operator<<(std::ostream& o, Response const& rhs) {
	
	o << rhs.responseStr();
	return (o);
}
