#include "CommonConfig.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

CommonConfig::CommonConfig() :
_clientMaxBodySize(1048576),
_autoindex("off") {

	this->_locations["/"] = "../www/html";
	this->_errorPages[404] = "../www/html/error/404.html";
	//NOTE: mettre valeurs par defaut APRES le parsing, SI le vecteur est vide (pareil pour les autres vecteurs)
	this->_index.push_back("index.html");
	this->_authorizedMethods.push_back("GET");
	this->_authorizedMethods.push_back("POST");
	this->_authorizedMethods.push_back("DELETE");
}

CommonConfig::CommonConfig(CommonConfig const& rhs) { *this = rhs; }

CommonConfig& CommonConfig::operator=(CommonConfig const& rhs) { 

	if (this != &rhs) {
		
		this->_clientMaxBodySize = rhs.getClientMaxBodySize();
		this->_errorPages = rhs.getErrorPages();
		this->_index = rhs.getIndex();
		this->_root = rhs.getRoot();
		this->_locations = rhs.getLocations();
		this->_autoindex = rhs.getAutoindex();
		this->_authorizedMethods = rhs.getAuthorizedMethods();
	}
	return *this;
}

CommonConfig::~CommonConfig() {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

size_t										CommonConfig::getClientMaxBodySize(void) const { return this->_clientMaxBodySize; }
 
std::map<int, std::string> const &			CommonConfig::getErrorPages(void) const { return this->_errorPages; }

std::vector<std::string> const & 			CommonConfig::getIndex(void) const { return this->_index; }

std::string const &							CommonConfig::getRoot(void) const { return this->_root; }

std::map<std::string, std::string> const &	CommonConfig::getLocations(void) const { return this->_locations; }

bool										CommonConfig::getAutoindex(void) const { return this->_autoindex; }

std::vector<std::string> const &			CommonConfig::getAuthorizedMethods(void) const { return this->_authorizedMethods; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	CommonConfig::setClientMaxBodySize(std::string const& line) {
	
	std::stringstream	stream(line);
	std::string			tmp;

	stream >> tmp >> this->_clientMaxBodySize;
}

void	CommonConfig::setErrorPage(std::string const &line) { 

	std::stringstream	stream(line);
	std::string			tmp;

	//todo
}

void	CommonConfig::setIndex(std::string const &line) {

	std::stringstream	stream(line);
	std::string			tmp;

	stream >> tmp;
	while (stream >> tmp) {
		this->_index.push_back(tmp);
	}
}

void	CommonConfig::setRoot(std::string const &line) { 

	std::stringstream	stream(line);
	std::string			tmp;

	stream >> tmp >> this->_root;
}

void	CommonConfig::setAutoindex(std::string const &line) {

(void)line;
// find on/off
}

void	CommonConfig::setAuthorizedMethods(std::string const& line) { 

	std::stringstream	stream(line);
	std::string			tmp;

	stream >> tmp;
	while (stream >> tmp) {
		this->_authorizedMethods.push_back(tmp);
	}
}

void	CommonConfig::setLocation(std::stringstream& stream) { 
	
	(void)stream;
//	go through lines until closing bracket
//	this->_locations[location] = path;


}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATOR OVERLOAD::

std::ostream &	operator<<(std::ostream & o, CommonConfig const & cc) {

	o << HIPURPLE << "clientMaxBodySize: " << HIGREEN << cc.getClientMaxBodySize() << NO_COLOR << std::endl;
	o << HIPURPLE << "errorPages: " << HIGREEN << print_map(cc.getErrorPages()) << NO_COLOR << std::endl;
	o << HIPURPLE << "index: " << HIGREEN << print_vector(cc.getIndex()) << NO_COLOR << std::endl;
	o << HIPURPLE << "root: " << HIGREEN << cc.getRoot() << NO_COLOR << std::endl;
	o << HIPURPLE << "locations: " << HIGREEN << print_map(cc.getLocations()) << NO_COLOR << std::endl;
	o << HIPURPLE << "autoindex: " << HIGREEN << cc.getAutoindex() << NO_COLOR << std::endl;
	o << HIPURPLE << "authorizedMethods: " << HIGREEN << print_vector(cc.getAuthorizedMethods()) << NO_COLOR << std::endl;
	return o;

}