#include "LocationContext.hpp"

LocationContext::LocationContext() : _maxBodySize(0), _alias(""), _root(""), _errorPages(), _authorizedMethods() {}

LocationContext::LocationContext(LocationContext const& rhs) { *this = rhs; }

LocationContext& LocationContext::operator=(LocationContext const& rhs) { 

	if (this != &rhs) {
		
		this->_maxBodySize = rhs.getMaxBodySize();
		this->_alias = rhs.getAlias();
		this->_root = rhs.getRoot();
		this->_errorPages = rhs.getErrorPages();
		this->_authorizedMethods = rhs.getAuthorizedMethods();
	}
	return *this;
}

LocationContext::~LocationContext() {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

size_t								LocationContext::getMaxBodySize(void) const { return this->_maxBodySize; }
std::string const&					LocationContext::getAlias(void) const { return this->_alias; }
std::string const&					LocationContext::getRoot(void) const { return this->_root; }
std::map<int, std::string> const&	LocationContext::getErrorPages(void) const { return this->_errorPages; }
std::vector<std::string> const&		LocationContext::getAuthorizedMethods(void) const { return this->_authorizedMethods; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	LocationContext::setMaxBodySize(size_t size) { this->_maxBodySize = size; }
void	LocationContext::setAlias(std::string const& alias) { this->_alias = alias; }
void	LocationContext::setRoot(std::string const& root) { this->_root = root; }
void	LocationContext::setErrorPage(std::map<int, std::string> const& errorPages) { this->_errorPages = errorPages; }
void	LocationContext::setAuthorizedMethods(std::vector<std::string> const& authorizedMethods) { this->_authorizedMethods = authorizedMethods; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATOR OVERLOAD::

std::ostream &	operator<<(std::ostream & o, LocationContext const & sc) {

	o << HIPURPLE << std::setw(21) << "LocationContext : "	<< NO_COLOR << std::endl;
	
	o << HIPURPLE << std::setw(21) << "maxBodySize : " 		<< HIGREEN << sc.getMaxBodySize() 				<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "alias : " 			<< HIGREEN << sc.getAlias() 					<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "root : " 			<< HIGREEN << sc.getRoot() 						<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "errorPages : " 		<< HIGREEN << print_map(sc.getErrorPages()) 	<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(21) << "authorizedMethods : " 		<< HIGREEN << print_vector(sc.getAuthorizedMethods())	<< NO_COLOR << std::endl;
	return o;
}