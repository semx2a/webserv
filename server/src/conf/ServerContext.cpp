#include "ServerContext.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

ServerContext::ServerContext() : AContext() {}

ServerContext::ServerContext(ServerContext const& rhs) : AContext() { *this = rhs; }

ServerContext& ServerContext::operator=(ServerContext const& rhs) { 

	if (this != &rhs) {

		AContext::operator=(rhs);
		this->_locations = rhs.locations();
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
int												ServerContext::port(void) const { return this->_listen.begin()->second; }

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

	o << HIPURPLE << std::setw(21) << "autoindex: " 		<< HIGREEN << std::boolalpha <<sc.autoindex()			<< RESET << std::endl;
	o << HIPURPLE << std::setw(21) << "maxBodySize: "		<< HIGREEN << sc.maxBodySize()							<< RESET << std::endl;
	o << HIPURPLE << std::setw(21) << "root: " 				<< HIGREEN << sc.root()									<< RESET << std::endl;
	o << HIPURPLE << std::setw(21) << "errorPages: " 		<< HIGREEN << utl::print_map(sc.errorPages()) 			<< RESET << std::endl;
	o << HIPURPLE << std::setw(21) << "locations: " 		<< HIGREEN << utl::print_map(sc.locations())			<< RESET << std::endl;
	o << HIPURPLE << std::setw(21) << "index: " 			<< HIGREEN << utl::print_vector(sc.index())				<< RESET << std::endl;
	o << HIPURPLE << std::setw(21) << "authorizedMethods: "	<< HIGREEN << utl::print_vector(sc.authorizedMethods()) << RESET << std::endl;
	o << HIPURPLE << std::setw(21) << "uploadFolder: " 		<< HIGREEN << sc.uploadFolder() 							<< RESET << std::endl;

	o << HIPURPLE << std::setw(21) << "listen: " 			<< HIGREEN << utl::print_map(sc.listen()) 				<< RESET << std::endl;
	o << HIPURPLE << std::setw(21) << "serverNames: " 		<< HIGREEN << utl::print_vector(sc.serverNames())		<< RESET << std::endl;

	return o;

}