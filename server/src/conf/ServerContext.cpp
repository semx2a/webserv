#include "ServerContext.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

ServerContext::ServerContext() : AContext() {}

ServerContext::ServerContext(ServerContext const& rhs) : AContext() { *this = rhs; }

ServerContext& ServerContext::operator=(ServerContext const& rhs) { 

	if (this != &rhs) {

		this->_autoindex = rhs.autoindex();
		this->_cgi = rhs.cgi();
		this->_maxBodySize = rhs.maxBodySize();
		this->_errorPages = rhs.errorPages();
		this->_index = rhs.index();
		this->_root = rhs.root();
		this->_locations = rhs.locations();
		this->_autoindex = rhs.autoindex();
		this->_authorizedMethods = rhs.authorizedMethods();
		this->_listen = rhs.listen();
		this->_serverNames = rhs.serverNames();
	}
	return *this;
}

ServerContext::~ServerContext() {}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

std::map<std::string, int> const &				ServerContext::listen(void) const { return this->_listen; }
std::map<std::string, LocationContext> const &	ServerContext::locations(void) const { return this->_locations; }
std::vector<std::string> const &				ServerContext::serverNames(void) const { return this->_serverNames; }


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	ServerContext::setListen(std::string const& ip, int port) { this->_listen[ip] = port; }
void	ServerContext::addLocation(std::string const& location, LocationContext const& locationContext) { this->_locations[location] = locationContext; }
void	ServerContext::setLocations(std::map<std::string, LocationContext> locationsMap) { this->_locations = locationsMap; }
void	ServerContext::setServerNames(std::vector<std::string> const& serverNames) { this->_serverNames = serverNames; }


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void	ServerContext::setDefaults() {

	AContext::setDefaults();
	if (this->_listen.empty())
		this->_listen["0.0.0.0"] = 80;
	if (this->_serverNames.empty())
		this->_serverNames.push_back("default");
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATOR OVERLOAD::

std::ostream &	operator<<(std::ostream & o, ServerContext const & sc) {

	o << HIPURPLE << std::setw(21) << "autoindex: " 		<< HIGREEN << std::boolalpha <<sc.autoindex()		<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "cgi: " 				<< HIGREEN << sc.cgi()								<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "maxBodySize: "		<< HIGREEN << sc.maxBodySize()						<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "root: " 				<< HIGREEN << sc.root()								<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "listen: " 			<< HIGREEN << print_map(sc.listen()) 				<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "errorPages: " 		<< HIGREEN << print_map(sc.errorPages()) 			<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "locations: " 		<< HIGREEN << print_map(sc.locations())				<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "index: " 			<< HIGREEN << print_vector(sc.index())				<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "authorizedMethods: "	<< HIGREEN << print_vector(sc.authorizedMethods()) 	<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "serverNames: " 		<< HIGREEN << print_vector(sc.serverNames())			<< NO_COLOR << std::endl;

	return o;

}