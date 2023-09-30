#include "LocationContext.hpp"

LocationContext::LocationContext() : AContext(), _alias("") {}

LocationContext::LocationContext(LocationContext const& rhs) : AContext() { *this = rhs; }

LocationContext& LocationContext::operator=(LocationContext const& rhs) { 

	if (this != &rhs) {
		
		this->_maxBodySize = rhs.maxBodySize();
		this->_alias = rhs.alias();
		this->_root = rhs.root();
		this->_errorPages = rhs.errorPages();
		this->_authorizedMethods = rhs.authorizedMethods();
	}
	return *this;
}

LocationContext::~LocationContext() {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

std::string const&					LocationContext::alias(void) const { return this->_alias; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	LocationContext::setAlias(std::string const& alias) { this->_alias = alias; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void	LocationContext::setDefaults() {

	AContext::setDefaults();
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATOR OVERLOAD::

std::ostream &	operator<<(std::ostream & o, LocationContext const & sc) {

	o << HIPURPLE << "------ LocationContext ------"	<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "maxBodySize : " 		<< HIGREEN << sc.maxBodySize() 						<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "alias : " 			<< HIGREEN << sc.alias() 							<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "root : " 			<< HIGREEN << sc.root() 								<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "errorPages : " 		<< HIGREEN << print_map(sc.errorPages()) 			<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "authorizedMethods : "<< HIGREEN << print_vector(sc.authorizedMethods())	<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(62) << "-----------------------------"	<< NO_COLOR << std::endl;

	return o;
}