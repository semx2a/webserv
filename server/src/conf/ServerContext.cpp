#include "ServerContext.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

ServerContext::ServerContext() : _autoindex("off"), _clientMaxBodySize(1048576) {

	this->_locations["/"] = "../www/html";
	this->_errorPages[404] = "../www/html/error/404.html";
	//NOTE: mettre valeurs par defaut APRES le parsing, SI le vecteur est vide (pareil pour les autres vecteurs)
	this->_index.push_back("index.html");
	this->_authorizedMethods.push_back("GET");
	this->_authorizedMethods.push_back("POST");
	this->_authorizedMethods.push_back("DELETE");
	//If the directive is not present then either *:80 is used if nginx runs with the superuser privileges, or *:8000 otherwise.
	this->_listen["*"] = 80;
	this->_serverNames.push_back("localhost");
}

ServerContext::ServerContext(ServerContext const& rhs) { *this = rhs; }

ServerContext& ServerContext::operator=(ServerContext const& rhs) { 

	if (this != &rhs) {
		
		this->_clientMaxBodySize = rhs.getClientMaxBodySize();
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

size_t										ServerContext::getClientMaxBodySize(void) const { return this->_clientMaxBodySize; }
 
std::map<int, std::string> const &			ServerContext::getErrorPages(void) const { return this->_errorPages; }

std::vector<std::string> const & 			ServerContext::getIndex(void) const { return this->_index; }

std::string const &							ServerContext::getRoot(void) const { return this->_root; }

std::map<std::string, std::string> const &	ServerContext::getLocations(void) const { return this->_locations; }

bool										ServerContext::getAutoindex(void) const { return this->_autoindex; }

std::vector<std::string> const &			ServerContext::getAuthorizedMethods(void) const { return this->_authorizedMethods; }

std::map<std::string, int> const &			ServerContext::getListen(void) const { return this->_listen; }

std::vector<std::string> const &			ServerContext::getServerNames(void) const { return this->_serverNames; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	ServerContext::setClientMaxBodySize(std::string const& line) {
	
	std::stringstream	stream(line);
	std::string			tmp;

	stream >> tmp >> this->_clientMaxBodySize;
}

void	ServerContext::setErrorPage(std::string const &line) { 

	std::stringstream	stream(line);
	std::string			tmp;

	//todo
}

void	ServerContext::setIndex(std::string const &line) {

	std::stringstream	stream(line);
	std::string			tmp;

	stream >> tmp;
	while (stream >> tmp) {
		this->_index.push_back(tmp);
	}
}

void	ServerContext::setRoot(std::string const &line) { 

	std::stringstream	stream(line);
	std::string			tmp;

	stream >> tmp >> this->_root;
}

void	ServerContext::setAutoindex(std::string const &line) {

(void)line;
// find on/off
}

void	ServerContext::setAuthorizedMethods(std::string const& line) { 

	std::stringstream	stream(line);
	std::string			tmp;

	stream >> tmp;
	while (stream >> tmp) {
		this->_authorizedMethods.push_back(tmp);
	}
}

void	ServerContext::setLocation(std::stringstream& stream) { 
	
	(void)stream;
//	go through lines until closing bracket
//	this->_locations[location] = path;


}

void	ServerContext::setListen(std::string const& ip, int port) { this->_listen[ip] = port; }

void	ServerContext::setServerName(std::string const& serverName) { this->_serverNames.push_back(serverName); }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATOR OVERLOAD::

std::ostream &	operator<<(std::ostream & o, ServerContext const & sc) {

	o << HIPURPLE << std::setw(21) << "clientMaxBodySize: "	<< HIGREEN << sc.getClientMaxBodySize()					<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "errorPages: " 		<< HIGREEN << print_map(sc.getErrorPages()) 			<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "index: " 			<< HIGREEN << print_vector(sc.getIndex())				<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "root: " 				<< HIGREEN << sc.getRoot()								<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "locations: " 		<< HIGREEN << print_map(sc.getLocations())				<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "autoindex: " 		<< HIGREEN << sc.getAutoindex()							<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "authorizedMethods: "	<< HIGREEN << print_vector(sc.getAuthorizedMethods()) 	<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "listen: " 			<< HIGREEN << print_map(sc.getListen()) 				<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "serverNames: " 		<< HIGREEN << print_vector(sc.getServerNames())			<< NO_COLOR << std::endl;

	return o;

}