#include "Response.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Response::Response() {

	//TODO
}

Response::Response(Request const& request, ServerContext const& serverContext) : _request(request), _serverContext(serverContext) {

	try {
		_expandTarget();
		if (_request.method() == "GET") {
			handleGet();
		_statusLine = "HTTP/1.1 200 OK";
		_statusLine += CRLF;
		}
	}
	catch (Response::HttpError& e) {
  		#ifdef DEBUG_RESPONSE
		std::cout << "HttpError: " << e.statusCode() << std::endl;
		#endif

		StatusLine	statusLine(e.statusCode());
		statusLine.build();
		this->_statusLine = statusLine.getMessage();
		std::stringstream bodyError;
		bodyError << "<html><body><h1>" << e.statusCode() << "</h1></body></html>";
		_body = bodyError.str();
	}
	std::stringstream headers;
	headers << "Content-Type: text/html" << CRLF;
	headers << "Content-Length: " << _body.length() << CRLF;
	headers << CRLF;
	_headers = headers.str();
	_responseStr = _statusLine + _headers + _body;
}

Response::Response(Response const& rhs) {

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


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

Request const&		Response::request() const { return _request; }

std::string const&	Response::statusLine() const { return _statusLine; }
std::string const&	Response::headers() const { return _headers; }
std::string const&	Response::body() const { return _body; }
std::string const&	Response::responseStr() const { return _responseStr; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMMON::

void Response::_expandTarget() {

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

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: GET::

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
		throw Response::HttpError("404");
		return ;
	}

	std::stringstream	bodyContent;
	std::string			line;
	while (std::getline(file, line) && !file.eof())
		bodyContent << line << std::endl;
	file.close();
	_body = bodyContent.str();
}

bool	Response::_isDirectory() {

	return _path.find_last_of('/') == _path.size() - 1;
}

bool	Response::_isCgi() {

	return _path.find(".php") != std::string::npos;
}

void	Response::handlePost() {

	
}

void	Response::handleDelete(void) {

	if (access(this->_path.c_str(), F_OK) == -1)
		throw HttpError("404");
	else if (access(this->_path.c_str(), W_OK | X_OK) == -1)
		throw HttpError("403");
	if (std::remove(this->_path.c_str()) != 0)
		throw HttpError("204");
	throw HttpError("200");	
}

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
			throw Response::HttpError("404");
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
	throw Response::HttpError("403");
}

void	Response::_autoIndex() {

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Entering AutoIndex" << std::endl;
	#endif
	DIR*				dir;
	struct dirent*		entry;
	std::stringstream	fileTree;

	dir = opendir(_path.c_str());
	if (!dir) {
		
		std::cout << "Could not open directory " << _path << std::endl;
		return ;
		//throw std::runtime_error("Could not open directory " + _path);
	}

	while ((entry = readdir(dir)) != NULL) {

		fileTree << entry->d_name;	
	}
	
	closedir(dir);

	std::cout << fileTree.str() << std::endl;
}

void	Response::_runCgi() {

	#ifdef DEBUG_RESPONSE
	std::cout << "[DEBUG] Entering runCgi" << std::endl;
	#endif

	
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::OUTPUT OPERATOR OVERLOAD::

std::ostream& operator<<(std::ostream& o, Response const& rhs) {
	o << rhs.responseStr();
	return (o);
}
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ERRORS::

Response::HttpError::HttpError(std::string const& statusCode) : _statusCode(statusCode) {}
Response::HttpError::~HttpError() throw() {}

std::string const&	Response::HttpError::statusCode() const { return _statusCode; }

const char*			Response::HttpError::what() const throw() { return _statusCode.c_str(); }