#include "Response.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Response::Response() {

	//TODO
}

Response::Response(Request const& request, ServerContext const& serverContext) : _request(request), _serverContext(serverContext) {

	//TODO
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

std::string const&	Response::responseStr() { 
	
//	std::stringstream	bodyStream;
//	
//	bodyStream	<< "<!DOCTYPE html>\n"
//			<< "<html>\n"
//			<< "<head>\n"
//			<< "<title>Page Title</title>\n"
//			<< "</head>\n"
//			<< "<body>\n"
//			<< "\n"
//			<< "<h1>This is a Heading</h1>\n"
//			<< "<p>This is a paragraph.</p>\n"
//			<< "\n"
//			<< "</body>\n"
//			<< "</html>\n";
//	_body = bodyStream.str();
	
	std::stringstream res;

	res << _request.version() + " ";
	res << "200 "; //_statusCode + " ";
	res << "OK"; // _statusMessage(this->_response->statusCode());
	res << CRLF;
	res << "Content-Type: " << "text/html" << CRLF;
	res << "Content-Length: " << _body.size() << CRLF;
	res << CRLF;
	res << _body;

	_responseStr = res.str();
	return _responseStr;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMMON::

void Response::_expandTarget() {

	std::string target = _request.target();
	t_locationIterator it = _serverContext.locations().find(target);
	t_locationIterator itEnd = _serverContext.locations().end();

	std::string path;

	if (it != itEnd) {

		std::string root = it->second.root();
		std::string alias = it->second.alias();
		if (!root.empty()) {
			path = root + target;
		}
		else if (!alias.empty()) {
			path = alias;
		}
	}

	if (path.empty()) { // if not found in locations or no root or alias
		path = _serverContext.root() + target;
	}

	size_t pos = 0;
	while ((pos = path.find("//", pos)) != std::string::npos) {
		path.replace(pos, 2, "/");
	}
	_path = path;

	#ifdef DEBUG_RESPONSE
	std::cout << "Path: " << _path << std::endl;
	#endif
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: GET::

void	Response::_handleGet () {

	if (_path.find('/') == _path.size() - 1) { // directory
		_expandDirectory();
	}

	std::ifstream	file(_path.c_str());

	if (!file.is_open()) {
		//throw std::runtime_error("Could not open " +  _path + " file"); 
		// À fix: le serveur quitte lorsqu'il ne trouve pas le path (ex ../www/html/favicon.ico)
		// Devrait renvoyer erreur 404
		return ;
	}
	std::stringstream	bodyContent;
	std::string			line;
	
	while (std::getline(file, line) && !file.eof())
		bodyContent << line << std::endl;
	
	file.close();
	_body = bodyContent.str();
}

void	Response::_expandDirectory() {

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

		else {
			_assignIndex(_serverContext.index());
		}
	}
}

void	Response::_assignIndex(std::vector<std::string> const& indexVec) {

	for (size_t i = 0; i < indexVec.size(); i++) {
		
		_path += indexVec[i];
		
		std::ifstream	file(_path.c_str());
		if (file.is_open())
			return;
			
		_path = _path.substr(0, _path.size() - indexVec[i].size());
	}
	//throw std::runtime_error("Could not open index file");
}

void	Response::_autoIndex() {

	std::cout << "Enter AutoIndex" << std::endl;
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

//::::::::::::::::::::::::::::::::::::::::::::::::::::OUTPUT OPERATOR OVERLOAD::

//std::ostream& operator<<(std::ostream& o, Response const& rhs) {
//	//o << rhs.responseStr();
//	//TODO
//	return (o);
//}
	