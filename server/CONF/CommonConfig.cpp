#include "CommonConfig.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

CommonConfig::CommonConfig() :
_clientMaxBodySize(1048576),
_errorPage("error_page.html"),
_index("index.neoserv.html"),
_autoindex("off") {

	this->_locations["/"] = "../www/html";
	this->_limit_except.push_back("GET");
	this->_limit_except.push_back("POST");
	this->_limit_except.push_back("DELETE");
}

CommonConfig::CommonConfig(CommonConfig const& rhs) { *this = rhs; }

CommonConfig& CommonConfig::operator=(CommonConfig const& rhs) { 

	if (this != &rhs) {

	}
	return *this;
}

CommonConfig::~CommonConfig() {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

size_t										CommonConfig::getClientMaxBodySize(void) const { return this->_clientMaxBodySize; }
 
std::string const &							CommonConfig::getErrorPage(void) const { return this->_errorPage; }

std::string const & 						CommonConfig::getIndex(void) const { return this->_index; }

std::map<std::string, std::string> const &	CommonConfig::getLocations(void) const { return this->_locations; }

bool										CommonConfig::getAutoindex(void) const { return this->_autoindex; }

std::vector<std::string> const &			CommonConfig::getLimitExcept(void) const { return this->_limit_except; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	CommonConfig::setClientMaxBodySize(size_t clientMaxBodySize) { this->_clientMaxBodySize = clientMaxBodySize; }

void	CommonConfig::setErrorPage(std::string const &errorPage) { this->_errorPage = errorPage; }

void	CommonConfig::setIndex(std::string const &index) { this->_index = index; }

void	CommonConfig::setLocation(std::string const &location, std::string const &path) { this->_locations[location] = path; }

void	CommonConfig::setAutoindex(bool autoindex) { this->_autoindex = autoindex; }

void	CommonConfig::setLimitExcept(std::vector<std::string> const &limit_except) { this->_limit_except = limit_except; }


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

