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

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::



// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATOR OVERLOAD::

std::ostream &	operator<<(std::ostream & o, SpecConfig const & sc) {

	o << RED << "listenIpPort = " << NO_COLOR << print_map(sc.getListenIpPort()) << std::endl;
	o << RED << "serverNames = " << NO_COLOR << print_vector(sc.getServerNames()) << std::endl;
	return o;
}