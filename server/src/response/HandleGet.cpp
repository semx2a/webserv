#include "HandleGet.hpp"

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

HandleGet::HandleGet(Request const& request, ServerContext const& serverContext) 
: AHandler (request, serverContext) {}



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
	setBody(bodyContent.str());
}
