#include "HandleGet.hpp"

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

HandleGet::HandleGet(Request const& request, ServerContext const& serverContext) 
: AHandler (request, serverContext) {}

HandleGet::HandleGet(HandleGet const& rhs) : AHandler(rhs) {

	if (this != &rhs)
		*this = rhs;
}

HandleGet::~HandleGet() {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMPARISON OPERATORS::

HandleGet& HandleGet::operator=(HandleGet const& rhs) {

	if (this != &rhs) {
		//TODO
	}
	return *this;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void	HandleGet::handle() {

	if (this->_path.find('/') == this->_path.size() - 1) { // directory
		
		if (_serverContext.getAutoindex() == true) {
			_handleAutoIndex();
			return ;
		}
		else {
			t_locationIterator it = _serverContext.getLocations().find(_request.getTarget());
			t_locationIterator itEnd = _serverContext.getLocations().end();

			std::string index;
			if (it != itEnd) {
				_assignIndex(it->second.getIndex());
			}
			else {
				_assignIndex(_serverContext.getIndex());
			}
		}
	}

	std::ifstream	file(this->_path.c_str());

	if (!file.is_open()) {
		//throw std::runtime_error("Could not open " +  this->_path + " file"); 
		// À fix: le serveur quitte lorsqu'il ne trouve pas le path (ex ../www/html/favicon.ico)
		// Devrait renvoyer erreur 404
		return ;
	}
	std::stringstream	bodyContent;
	std::string			line;
	
	while (std::getline(file, line) && !file.eof())
		bodyContent << line << std::endl;
	
	file.close();
	std::cout << "TODO: set body content" << std::endl;
	//setBody(bodyContent.str());
}

void	HandleGet::_assignIndex(std::vector<std::string> const& indexVec) {

	for (size_t i = 0; i < indexVec.size(); i++) {
		
		setPath(this->_path + indexVec[i]);
		
		std::ifstream	file(this->_path.c_str());
		if (file.is_open())
			return;
			
		setPath(this->_path.substr(0, this->_path.size() - indexVec[i].size()));
	}
	//throw std::runtime_error("Could not open index file");
}

void	HandleGet::_handleAutoIndex() {

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
