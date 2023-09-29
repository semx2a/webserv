#include "AHandler.hpp"

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

AHandler::AHandler(Request const& request, ServerContext const& serverContext) : _request(request), _serverContext(serverContext) {

	_setPath();
}

AHandler::AHandler(AHandler const& rhs) : _request(rhs.getRequest()), _serverContext(rhs.getServerContext()) {

	if (this != &rhs)
		*this = rhs;
}

AHandler::~AHandler() {}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMPARISON OPERATORS::

AHandler& AHandler::operator=(AHandler const& rhs) {

	if (this != &rhs) {
		//TODO
	}
	return *this;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

Request const&			AHandler::getRequest() const { return this->_request; }
ServerContext const&	AHandler::getServerContext() const { return this->_serverContext; }
StatusCodes const&		AHandler::getStatusCodes() const { return this->_statusCodes; }
MimeTypes const&		AHandler::getMimeTypes() const { return this->_mimeTypes; }
std::string const&		AHandler::getPath() const { return this->_path; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: SETTERS::

void	AHandler::setRequest(Request const& request) { this->_request = request; }
void	AHandler::setServerContext(ServerContext const& serverContext) { this->_serverContext = serverContext; }
void	AHandler::setStatusCodes(StatusCodes const& statusCodes) { this->_statusCodes = statusCodes; }
void	AHandler::setMimeTypes(MimeTypes const& mimeTypes) { this->_mimeTypes = mimeTypes; }
void	AHandler::setPath(std::string const& path) { this->_path = path; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void AHandler::_setPath() {

	std::string target = this->_request.getTarget();
	t_locationIterator it = this->_serverContext.getLocations().find(target);
	t_locationIterator itEnd = this->_serverContext.getLocations().end();

	std::string path;
	if (it != itEnd) {
		std::string root = it->second.getRoot();
		std::string alias = it->second.getAlias();
		if (!root.empty()) {
			path = root + target;
		}
		else if (!alias.empty()) {
			path = alias;
		}
	}
	if (path.empty()) { // if not found in locations or no root or alias
		path = this->_serverContext.getRoot() + target;
	}
	size_t pos = 0;
	while ((pos = path.find("//", pos)) != std::string::npos) {
		path.replace(pos, 2, "/");
	}
	setPath(path);

	#ifdef DEBUG_RESPONSE_HANDLER
	std::cout << "Path: " << this->_path << std::endl;
	#endif
}

void	AHandler::_handleAutoIndex() {

	std::cout << "Enter AutoIndex" << std::endl;
	DIR*				dir;
	struct dirent*		entry;
	std::stringstream	fileTree;

	dir = opendir(this->_path.c_str());
	if (!dir) {
		
		std::cout << "Could not open directory " << this->_path << std::endl;
		return ;
		//throw std::runtime_error("Could not open directory " + _path);
	}

	while ((entry = readdir(dir)) != NULL) {

		fileTree << entry->d_name;	
	}
	
	closedir(dir);

	std::cout << fileTree.str() << std::endl;
}

void	AHandler::_assignIndex(std::vector<std::string> const& indexVec) {

	for (size_t i = 0; i < indexVec.size(); i++) {
		
		setPath(this->_path + indexVec[i]);
		
		std::ifstream	file(this->_path.c_str());
		if (file.is_open())
			return;
			
		setPath(this->_path.substr(0, this->_path.size() - indexVec[i].size()));
	}
	//throw std::runtime_error("Could not open index file");
}


void	AHandler::_handlePost() {}

void	AHandler::_handleDelete() {}