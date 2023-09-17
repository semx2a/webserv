#include "CommonServerConfig.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

CommonServerConfig::CommonServerConfig() {}

CommonServerConfig::CommonServerConfig(std::string const& conf_file) :
_clientMaxBodySize(1048576),
_errorPage("error_page.html"),
_index("index.neoserv.html"),
_autoindex("off") {

	this->_locations["/"] = "../www/html";
	this->_limit_except.push_back("GET");
	this->_limit_except.push_back("POST");
	this->_limit_except.push_back("DELETE");
}

CommonServerConfig::CommonServerConfig(CommonServerConfig const& rhs) { *this = rhs; }

CommonServerConfig& CommonServerConfig::operator=(CommonServerConfig const& rhs) { 

	if (this != &rhs) {

	}
	return *this;
}

CommonServerConfig::~CommonServerConfig() {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

size_t										CommonServerConfig::getClientMaxBodySize(void) const { return this->_clientMaxBodySize; }
 
std::string const &							CommonServerConfig::getErrorPage(void) const { return this->_errorPage; }

std::string const & 						CommonServerConfig::getIndex(void) const { return this->_index; }

std::map<std::string, std::string> const &	CommonServerConfig::getLocations(void) const { return this->_locations; }

bool										CommonServerConfig::getAutoindex(void) const { return this->_autoindex; }

std::vector<std::string> const &			CommonServerConfig::getLimitExcept(void) const { return this->_limit_except; }


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	CommonServerConfig::setClientMaxBodySize(size_t clientMaxBodySize) { this->_clientMaxBodySize = clientMaxBodySize; }

void	CommonServerConfig::setErrorPage(std::string const &errorPage) { this->_errorPage = errorPage; }

void	CommonServerConfig::setIndex(std::string const &index) { this->_index = index; }

void	CommonServerConfig::setLocation(std::string const &location, std::string const &path) { this->_locations[location] = path; }

void	CommonServerConfig::setAutoindex(bool autoindex) { this->_autoindex = autoindex; }

void	CommonServerConfig::setLimitExcept(std::vector<std::string> const &limit_except) { this->_limit_except = limit_except; }


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

