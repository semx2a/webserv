#include "SpecConfig.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

SpecConfig::SpecConfig(void) {}

SpecConfig::SpecConfig(std::string const &SpecServerConf_file) : _listenIpPort(), _serverNames() {

	this->_listenIpPort["127.0.0.1"] = 80;
	this->_serverNames.push_back("localhost");
}

SpecConfig::SpecConfig(SpecConfig const &rhs) {

	*this = rhs;
}

SpecConfig &	SpecConfig::operator=(SpecConfig const & rhs) {

	if (this != &rhs) {


	}

	return *this;
} 

SpecConfig::~SpecConfig(void) {}


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

std::map<std::string, int> const &	SpecConfig::getListenIpPort(void) const { return this->_listenIpPort; }

std::vector<std::string> const &		SpecConfig::getServerNames(void) const { return this->_serverNames; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::


