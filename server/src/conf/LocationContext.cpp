#include "LocationContext.hpp"

LocationContext::LocationContext() : AContext(), _alias("") {}

LocationContext::LocationContext(LocationContext const& rhs) : AContext() { *this = rhs; }

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

std::string const&					LocationContext::getAlias(void) const { return this->_alias; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	LocationContext::setAlias(std::string const& alias) { this->_alias = alias; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void	LocationContext::setDefaults() {

	AContext::setDefaults();
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATOR OVERLOAD::

std::ostream &	operator<<(std::ostream & o, LocationContext const & sc) {

	o << HIPURPLE << "------ LocationContext ------"	<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "maxBodySize : " 		<< HIGREEN << sc.getMaxBodySize() 						<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "alias : " 			<< HIGREEN << sc.getAlias() 							<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "root : " 			<< HIGREEN << sc.getRoot() 								<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "errorPages : " 		<< HIGREEN << print_map(sc.getErrorPages()) 			<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "authorizedMethods : "<< HIGREEN << print_vector(sc.getAuthorizedMethods())	<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(62) << "-----------------------------"	<< NO_COLOR << std::endl;

	return o;
}