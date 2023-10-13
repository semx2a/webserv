#include "Response.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Response::Response(Request const& request, ResponseContext const& responseContext, HttpStatus const& status) : 
														_request(request), 
														_responseContext(responseContext), 
														_status(status),
														_path(responseContext.path()),
														_responseStr(),
														_customHeaders(),
														_filePath() {

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
	if (this != &rhs) {

//		this->setRequest(rhs.request());
//		this->setResponseContext(rhs.responseContext());
//		this->setStatus(rhs.status());
//		this->setBody(rhs.body());
//		this->setPath(rhs.path());
//		this->setBoundary(rhs.boundary());
//		this->setResponseStr(rhs.responseStr());
//		this->setCustomHeaders(rhs.customHeaders());
//		this->setFilePath(rhs.filePath());

	}
	return (*this);
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: GETTERS::

Request const&			Response::request() const { return _request; }
ResponseContext const&	Response::responseContext() const { return _responseContext; }
HttpStatus const&		Response::status() const { return _status; }
Body const&				Response::body() const { return _body; }

std::string const&		Response::path() const { return _path; }
std::string const&		Response::boundary() const { return _boundary; }
std::string const&		Response::responseStr() const { return _responseStr; }
std::string const&		Response::customHeaders() const { return _customHeaders; }
std::string const&		Response::filePath() const { return _filePath; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: SETTERS::

void	Response::setRequest(Request const& request) { _request = request; }
void	Response::setResponseContext(ResponseContext const& responseContext) { _responseContext = responseContext; }
void	Response::setStatus(HttpStatus const& status) { _status = status; }
void	Response::setBody(Body const& body) { _body = body; }

void	Response::setPath(std::string const& path) { _path = path; }
void	Response::setBoundary(std::string const& boundary) { _boundary = boundary; }
void	Response::setResponseStr(std::string const& responseStr) { _responseStr = responseStr; }
void	Response::setCustomHeaders(std::string const& customHeaders) { _customHeaders = customHeaders; }
void	Response::setFilePath(std::string const& filePath) { _filePath = filePath; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ENTRYPOINT::

void	Response::buildResponse() {

	StatusCodes					statusCodes;
	Response::MethodsMap::type	methodsMap = _initMethods();
 
	try {
        if (_status.statusCode().find_first_of("45") == 0)
            throw HttpStatus(_status.statusCode());

		_checkAuthorizedMethod();
		if (methodsMap.find(_request.method()) == methodsMap.end())
			throw HttpStatus("501");

		(this->*methodsMap[_request.method()])();
		if (_status.statusCode().empty())
			_status.setStatusCode("200");
	}
	catch (HttpStatus& e) {
		_status.setStatusCode(e.statusCode());
		_handleError();
	}

 	StatusLine	statusLine(_status.statusCode(), statusCodes.getReasonPhrase(_status.statusCode()));
	Headers		headers(this->path(), _body.getContentLength(), this->_customHeaders, this->responseContext(), this->_filePath);

	_responseStr = statusLine.getContent() + headers.getContent() + _body.getContent();
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

	if (utl::isDirectory(this->_path)) {
		if (_responseContext.autoindex() == "on") {
			_autoIndex();
			return ;
		}
		this->_expandDirectory();
	}
	std::cout << "[DEBUG] handleGet: path: " << _path << std::endl;
	if (_responseContext.isCgi()
		|| _path.find(".php") != std::string::npos
		|| _path.find(".py") != std::string::npos) {
		_runCgi();
		if (!this->_body.getContent().empty())
			return ;
	}

	std::ifstream	file(_path.c_str());
	if (!file.is_open()) {
		throw HttpStatus("404");
	}
	this->_body.build(utl::fileToStr(file));
}

void	Response::_handleUpload() {

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Post: entering handleUpload..." << std::endl;
	#endif

	if (_path.find("/") == _path.size() - 1 
		and utl::createDirectory(this->_path + this->responseContext().uploadFolder()) == false)
		throw HttpStatus("500");
		
	std::vector<char> postData = _request.body();
	
	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Post: entering handleUpload..." << std::endl;
	std::cout << "[DEBUG] Post: postData: " << std::string(postData.begin(), postData.end()) << std::endl;
	//sleep(2);
	#endif
		
	size_t			filenamePos = utl::searchVectorChar(postData, "filename=\"", 0);
	size_t			filenameSize = utl::searchVectorChar(postData, "\"", filenamePos + 11) - filenamePos - 11;
	std::string		filename(postData.begin() + filenamePos + 11, postData.begin() + filenameSize);
	std::string		filepath = this->responseContext().root() + this->responseContext().uploadFolder() + filename;
	std::ofstream	file(filename.c_str(), std::ios::app);

	if (!file.is_open())
		throw HttpStatus("500");

	file.write(&postData[0], postData.size());
	file.close();
}
		
void	Response::_postData() {

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Entering postData" << std::endl;
	#endif

	std::string			boundary = this->_request.boundary();
	std::vector<char>	body = _request.body();
	int					i = 0;

	while ((i = utl::searchVectorChar(body, boundary.c_str(), i)) != -1) {
		
		i += boundary.length() + 1;
		if (utl::searchVectorChar(body, "filename=", i) == -1) {
			break ;
		}
		if (utl::searchVectorChar(body, "filename=", i) > utl::searchVectorChar(body, boundary.c_str(), i) || body[utl::searchVectorChar(body, "filename=", i) + 10] == '\"')
			continue ;
		i = utl::searchVectorChar(body, "filename=", i) + 10;

		std::string			filename(body.begin() + i, body.begin() + utl::searchVectorChar(body, "\"", i));
		std::vector<char>	content(body.begin() + utl::searchVectorChar(body, "\r\n\r\n", i) + 4,
									body.begin() + utl::searchVectorChar(body, "\r\n\r\n", i) + 4 + 
									utl::searchVectorChar(body, ("\r\n--" + boundary).c_str(), i) -
									utl::searchVectorChar(body, "\r\n\r\n", i) - 4);
		
		_filePath = this->responseContext().root() + this->responseContext().uploadFolder();

		#ifdef DEBUG_RESPONSE
		std::cout << "[DEBUG] " << BOLD << "filename: " << RESET << filename << std::endl;
		std::cout << "[DEBUG] " << BOLD << "filepath: " << RESET << filepath << std::endl;
		#endif

		if (utl::createFile(_filePath, content, filename) == false)
			throw HttpStatus("500");

		_filePath += filename;
		this->_status.setStatusCode("201");
	}
}

void	Response::_handlePost() {

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Entering handlePost" << std::endl;
	#endif

	if (this->_request.body().empty()) {
		throw HttpStatus("400");
	}
	if (this->responseContext().isCgi()) {
		#ifdef DEBUG_RESPONSE
		std::cout << "[DEBUG] Post : calling runCgi..." << std::endl;
		#endif
		this->_runCgi();
	}
	if (utl::isDirectory(this->_path)) {
		if (_responseContext.autoindex() == "on") {
			_autoIndex();
			return ;
		}
		this->_expandDirectory();
	}

	if (access(this->path().c_str(), F_OK) == -1) {
		throw HttpStatus("404");
	}
	else if (access(this->path().c_str(), W_OK) == -1)
		throw HttpStatus("403");
	
//	if (not _boundary.empty()) {
	this->_postData();
	//}
//	else {
//		this->_handleUpload();
//	}

	this->_status.setStatusCode("201");
}

void	Response::_handleDelete() {

 	if (access(this->_path.c_str(), F_OK) == -1)
		throw HttpStatus("404");
	else if (_path[_path.size() - 1] == '/' || access(this->_path.c_str(), W_OK) == -1)
		throw HttpStatus("403");
	else if (_responseContext.isCgi())
		this->_runCgi();
	if (std::remove(this->_path.c_str()))
		throw HttpStatus("500");
	throw HttpStatus("204");
}

void	Response::_handleError() {

	int statusCode = std::atoi(_status.statusCode().c_str());

	std::string content;
	if (_responseContext.errorPages().find(statusCode) != _responseContext.errorPages().end()) {

		this->setPath(this->responseContext().errorPages().find(statusCode)->second);

		#ifdef DEBUG_RESPONSE
		std::cout << "[DEBUG ERROR] path: " << _path << std::endl;
		#endif

		std::ifstream	file(this->path().c_str());
		if (file.is_open())
			content = utl::fileToStr(file);
	}
	else {
		std::stringstream bodyContent;
		bodyContent	<< "<html><body><h1>" 
					<< _status.statusCode() 
					<< "</h1></body></html>";
		content = bodyContent.str();
	}
	this->_customHeaders.append("Content-Type: text/html");
	this->_customHeaders.append(CRLF);
	this->_body.build(content);
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: EXPANSION::

void	Response::_checkAuthorizedMethod() {

	std::vector<std::string>::const_iterator it = std::find(_responseContext.authorizedMethods().begin(), _responseContext.authorizedMethods().end(), _request.method());
	if (it == _responseContext.authorizedMethods().end()) {
		throw HttpStatus("405");
	}
}

void	Response::_expandDirectory() {

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Target is a directory" << std::endl;
	#endif

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

	DIR *dir = opendir(this->path().c_str());

	if (dir == NULL)
		throw HttpStatus("404");

	std::string to_remove = this->responseContext().serverContext().pwd();
	std::string title = "Index of http://www." + this->path().substr(to_remove.size(), this->path().size() - to_remove.size());
	std::stringstream page;
	page 	<< "<!DOCTYPE html>" << std::endl
			<< "<html>" << std::endl
			<< "<head>" << std::endl
			<< "<title>webserv</title>" <<std::endl
			<< "</head>" << std::endl
			<< "<body>" <<std::endl
			<< "<h1>"
			<< title
			<< "</h1>" << std::endl
			<< "<p>" << std::endl;
	
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;
		page << "<a href=\"" << entry->d_name << "\">" << entry->d_name << "</a><br>" << std::endl;
	}

	closedir(dir);

	page	<< "</p>" << std::endl
			<< "</body>" << std::endl
			<< "</html>" << std::endl;

	this->setBody(page.str());
}

void	Response::_runCgi() {

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Entering runCgi" << std::endl;
	#endif

	CGI	cgi(_path, _request, _responseContext);
	cgi.execute();
	if (!cgi.output().empty()) {

 		Request		request;

		std::vector<char>	str_vec(cgi.output().begin(), cgi.output().end());
		std::string			str(str_vec.begin(), str_vec.end());
		
		std::istringstream	stream(str);
		std::string			line;
		if (str.empty())
			return ;
		while (std::getline(stream, line)) {

			if (line.empty() || line == "\r")
				break;
			this->_customHeaders.append(line + CRLF);
		}
		if (!stream.eof())
			request.parseBody(str_vec);

		_body.build(request.body());
	}
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::OUTPUT OPERATOR OVERLOAD::

std::ostream& operator<<(std::ostream& o, Response const& rhs) {
	
	o << rhs.responseStr();
	return (o);
}
