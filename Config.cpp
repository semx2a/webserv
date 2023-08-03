#include "Config.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Config::Config () {

	ports.push_back (8080);
}

Config::Config (std::string const& conf_file) {

	(void) conf_file;
	ports.push_back (8080);
}

Config::Config (Config const& rhs) { *this = rhs; }

Config& Config::operator= (Config const& rhs) { 

	if (this != &rhs) {

	}
	return *this;
}

Config::~Config() {
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

std::vector <int>&	Config::getPorts () { return ports; }
