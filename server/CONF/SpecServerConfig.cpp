#include "SpecServerConfig.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

SpecServerConfig::SpecServerConfig(void) {}

SpecServerConfig::SpecServerConfig(std::string const &SpecServerConf_file) : _listenIpPort(), _serverNames() {

	this->_listenIpPort["127.0.0.1"] = 80;
	this->_serverNames.push_back("localhost");
}

SpecServerConfig::SpecServerConfig(SpecServerConfig const &rhs) {

	*this = rhs;
}

SpecServerConfig &	SpecServerConfig::operator=(SpecServerConfig const & rhs) {

	if (this != &rhs) {


	}

	return *this;
} 

SpecServerConfig::~SpecServerConfig(void) {}


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

std::map<std::string, int> &	SpecServerConfig::getListenIpPort(void) const { return this->_listenIpPort; }

std::vector<std::string> &		SpecServerConfig::getServerNames(void) const { return this->_serverNames; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::


