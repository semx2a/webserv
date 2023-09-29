#include "ResponseHandler.hpp"

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

ResponseHandler::ResponseHandler() : _response(NULL) {}

ResponseHandler::ResponseHandler(Response* response) : _response(response) {}

ResponseHandler::ResponseHandler(ResponseHandler const& rhs) : _response(rhs.getResponse()) {

	if (this != &rhs)
		*this = rhs;
}

ResponseHandler::~ResponseHandler() {}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMPARISON OPERATORS::

ResponseHandler& ResponseHandler::operator=(ResponseHandler const& rhs) {

	if (this != &rhs) {
		this->setResponse(rhs.getResponse());
	}
	return *this;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

Response*	ResponseHandler::getResponse() const { return this->_response; }


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: SETTERS::

void	ResponseHandler::setResponse(Response* response) { this->_response = response; }


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void	ResponseHandler::handleResponse() {

	// TODO :check authorized methods
	_checkTarget();
	if (_response->getRequest().getMethod() == "GET")
		this->_handleGet();
	else if (_response->getRequest().getMethod() == "POST")
		this->_handlePost();
	else if (_response->getRequest().getMethod() == "DELETE")
		this->_handleDelete();
	else
		throw std::runtime_error("Method not implemented");
}

void	ResponseHandler::_checkTarget() {

	std::string			target	= _response->getRequest().getTarget();
	t_locationIterator	it 		= _response->getServerContext().getLocations().find(target);
	t_locationIterator	itEnd	= _response->getServerContext().getLocations().end();

	if (it != itEnd) {
		std::string root = it->second.getRoot();
		std::string alias = it->second.getAlias();
		#ifdef DEBUG_RESPONSE_HANDLER
			std::cout << "Root: " << root << std::endl;
			std::cout << "Alias: " << alias << std::endl;
		#endif
		if (not root.empty())
			_response->setTargetFinalPath(root + target.substr(1));
		else if (not alias.empty())
			_response->setTargetFinalPath(alias);
	}
	if (_response->getTargetFinalPath().empty()) { // if not found in locations or no root or alias
		_response->setTargetFinalPath(_response->getServerContext().getRoot() + target.substr(1));
	}
	if (target.find("?") != std::string::npos) {
		_response->setCgi(true);
	}
	#ifdef DEBUG_RESPONSE_HANDLER
		std::cout << "Target: " << target << std::endl;
		std::cout << "Target final path: " << _response->getTargetFinalPath() << std::endl;
	#endif
}

void	ResponseHandler::_handleAutoIndex() {

	std::cout << "Enter AutoIndex" << std::endl;
	DIR*				dir;
	struct dirent*		entry;
	std::stringstream	fileTree;

	dir = opendir(_response->getTargetFinalPath().c_str());
	if (!dir) {
		
		std::cout << "Could not open directory " << _response->getTargetFinalPath() << std::endl;
		return ;
		//throw std::runtime_error("Could not open directory " + _response->getTargetFinalPath());
	}

	while ((entry = readdir(dir)) != NULL) {

		fileTree << entry->d_name;		
	}
	
	closedir(dir);

	std::cout << fileTree.str() << std::endl;
}

void	ResponseHandler::_assignIndex(std::vector<std::string> const& indexVec) {

	for (size_t i = 0; i < indexVec.size(); i++) {
		
		_response->setTargetFinalPath(_response->getTargetFinalPath() + indexVec[i]);
		
		std::ifstream	file(_response->getTargetFinalPath().c_str());
		if (file.is_open())
			return;
			
		_response->setTargetFinalPath(_response->getTargetFinalPath().substr(0, _response->getTargetFinalPath().size() - indexVec[i].size()));
	}
	//throw std::runtime_error("Could not open index file");
}

void	ResponseHandler::_handleGet() {

	if (_response->getTargetFinalPath().find('/') == _response->getTargetFinalPath().size() - 1) { // directory
		
		if (_response->getServerContext().getAutoindex() == true) {
			_handleAutoIndex();
			return ;
		}
		else {
			t_locationIterator it = _response->getServerContext().getLocations().find(_response->getRequest().getTarget());
			t_locationIterator itEnd = _response->getServerContext().getLocations().end();

			std::string index;
			if (it != itEnd) {
				_assignIndex(it->second.getIndex());
			}
			else {
				_assignIndex(_response->getServerContext().getIndex());
			}
		}
	}

	std::ifstream	file(_response->getTargetFinalPath().c_str());

	if (!file.is_open()) {
		//throw std::runtime_error("Could not open " +  _response->getTargetFinalPath() + " file"); 
		// À fix: le serveur quitte lorsqu'il ne trouve pas le path (ex ../www/html/favicon.ico)
		// Devrait renvoyer erreur 404
		return ;
	}
	std::stringstream	bodyContent;
	std::string			line;
	
	while (std::getline(file, line) && !file.eof())
		bodyContent << line << std::endl;
	
	file.close();
	_response->setBodyContent(bodyContent.str());
}

void	ResponseHandler::_handlePost() {}

void	ResponseHandler::_handleDelete() {}