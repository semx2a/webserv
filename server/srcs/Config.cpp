#include "../inc/Config.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Config::Config() {

	_ports.push_back(8080);
	_ports.push_back(8081);
	_ports.push_back(8082);
	_ports.push_back(8083);
	_ports.push_back(8084);
}

Config::Config(std::string const& conf_file) {

	(void) conf_file;
	_ports.push_back(8080);
	_ports.push_back(8081);
	_ports.push_back(8082);
	_ports.push_back(8083);
	_ports.push_back(8084);
}

Config::Config(Config const& rhs) { *this = rhs; }

Config& Config::operator=(Config const& rhs) { 

	if(this != &rhs) {

	}
	return *this;
}

Config::~Config() {
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

std::vector<int>&			Config::getPorts() { return _ports; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

