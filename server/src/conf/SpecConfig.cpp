#include "SpecConfig.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

SpecConfig::SpecConfig() : CommonConfig(), _listenIpPort(), _serverNames() {

	this->_listenIpPort["127.0.0.1"] = 80;
	this->_serverNames.push_back("localhost");
}

SpecConfig::SpecConfig(SpecConfig const &rhs) : CommonConfig() {

	*this = rhs;
}

SpecConfig &	SpecConfig::operator=(SpecConfig const & rhs) {

	if (this != &rhs) {
		
		this->_listenIpPort = rhs.getListenIpPort();
		this->_serverNames = rhs.getServerNames();
	}
	return *this;
} 

SpecConfig::~SpecConfig(void) {}


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

std::map<std::string, int> const &		SpecConfig::getListenIpPort(void) const { return this->_listenIpPort; }

std::vector<std::string> const &		SpecConfig::getServerNames(void) const { return this->_serverNames; }

void	SpecConfig::setListIpPort(std::string const& line) {
	
	std::stringstream	stream(line);
	std::string			tmp;
	std::string			ip;
	int					port;

	stream >> tmp >> ip >> port;
	this->_listenIpPort[ip] = port;
}

void	SpecConfig::setServerName(std::string const& line) {

	std::stringstream	stream(line);
	std::string			tmp;
	std::string			serverName;

	stream >> tmp >> serverName;
	this->_serverNames.push_back(serverName);
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::



// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATOR OVERLOAD::

std::ostream &	operator<<(std::ostream & o, SpecConfig const & sc) {

	o << HIPURPLE << "listenIpPort = " << HIGREEN << print_map(sc.getListenIpPort()) << NO_COLOR << std::endl;
	o << HIPURPLE << "serverNames = " << HIGREEN << print_vector(sc.getServerNames()) << NO_COLOR << std::endl;
	return o;
}