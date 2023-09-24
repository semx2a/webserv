#include "ServerContext.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

ServerContext::ServerContext() : AContext() {

//	this->_locations["/"] = "../www/html";
//	this->_errorPages[404] = "../www/html/error/404.html";
//	//NOTE: mettre valeurs par defaut APRES le parsing, SI le vecteur est vide (pareil pour les autres vecteurs)
//	this->_index.push_back("index.html");
//	this->_authorizedMethods.push_back("GET");
//	this->_authorizedMethods.push_back("POST");
//	this->_authorizedMethods.push_back("DELETE");
//	//If the directive is not present then either *:80 is used if nginx runs with the superuser privileges, or *:8000 otherwise.
//	this->_listen["*"] = 80;
//	this->_serverNames.push_back("localhost");
}

ServerContext::ServerContext(ServerContext const& rhs) : AContext() { *this = rhs; }

ServerContext& ServerContext::operator=(ServerContext const& rhs) { 

	if (this != &rhs) {

		this->_autoindex = rhs.getAutoindex();
		this->_cgi = rhs.getCgi();
		this->_maxBodySize = rhs.getMaxBodySize();
		this->_errorPages = rhs.getErrorPages();
		this->_index = rhs.getIndex();
		this->_root = rhs.getRoot();
		this->_locations = rhs.getLocations();
		this->_autoindex = rhs.getAutoindex();
		this->_authorizedMethods = rhs.getAuthorizedMethods();
		this->_listen = rhs.getListen();
		this->_serverNames = rhs.getServerNames();
	}
	return *this;
}

ServerContext::~ServerContext() {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

std::map<std::string, int> const &				ServerContext::getListen(void) const { return this->_listen; }
std::map<std::string, LocationContext> const &	ServerContext::getLocations(void) const { return this->_locations; }
std::vector<std::string> const &				ServerContext::getServerNames(void) const { return this->_serverNames; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	ServerContext::setListen(std::string const& ip, int port) { this->_listen[ip] = port; }
void	ServerContext::addLocation(std::string const& location, LocationContext const& locationContext) { this->_locations[location] = locationContext; }
void	ServerContext::setLocations(std::map<std::string, LocationContext> locationsMap) { this->_locations = locationsMap; }
void	ServerContext::setServerNames(std::vector<std::string> const& serverNames) { this->_serverNames = serverNames; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATOR OVERLOAD::

std::ostream &	operator<<(std::ostream & o, ServerContext const & sc) {

	o << HIPURPLE << std::setw(21) << "autoindex: " 		<< HIGREEN << std::boolalpha <<sc.getAutoindex()		<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "cgi: " 				<< HIGREEN << sc.getCgi()								<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "maxBodySize: "		<< HIGREEN << sc.getMaxBodySize()						<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "root: " 				<< HIGREEN << sc.getRoot()								<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "listen: " 			<< HIGREEN << print_map(sc.getListen()) 				<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "errorPages: " 		<< HIGREEN << print_map(sc.getErrorPages()) 			<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "locations: " 		<< HIGREEN << print_map(sc.getLocations())				<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "index: " 			<< HIGREEN << print_vector(sc.getIndex())				<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "authorizedMethods: "	<< HIGREEN << print_vector(sc.getAuthorizedMethods()) 	<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "serverNames: " 		<< HIGREEN << print_vector(sc.getServerNames())			<< NO_COLOR << std::endl;

	return o;

}