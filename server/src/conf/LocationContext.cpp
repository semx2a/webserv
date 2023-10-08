#include "LocationContext.hpp"

LocationContext::LocationContext() : 
AContext(), 
								_alias(""), 
								_isCgi(false) {}

LocationContext::LocationContext(LocationContext const& rhs) : AContext() { *this = rhs; }

LocationContext& LocationContext::operator=(LocationContext const& rhs) {

	if (this != &rhs) {
		
		AContext::operator=(rhs);
		this->_alias = rhs.alias();
		this->_isCgi = rhs.isCgi();
	}
	return *this;
}

LocationContext::~LocationContext() {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

std::string const&		LocationContext::alias(void) const { return this->_alias; }
bool					LocationContext::isCgi(void) const { return this->_isCgi; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	LocationContext::setAlias(std::string const& alias) { this->_alias = alias; }
void	LocationContext::setIsCgi(bool isCgi) { this->_isCgi = isCgi; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void	LocationContext::setDefaults() {

	AContext::setDefaults();
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATOR OVERLOAD::

std::ostream &	operator<<(std::ostream & o, LocationContext const & sc) {

	o << HIPURPLE << "------ LocationContext ------"	<< RESET << std::endl;
	o << HIPURPLE << std::setw(52) << "maxBodySize : " 		<< HIGREEN << sc.maxBodySize() 							<< RESET << std::endl;
	o << HIPURPLE << std::setw(52) << "alias : " 			<< HIGREEN << sc.alias() 								<< RESET << std::endl;
	o << HIPURPLE << std::setw(52) << "root : " 			<< HIGREEN << sc.root() 								<< RESET << std::endl;
	o << HIPURPLE << std::setw(52) << "errorPages : " 		<< HIGREEN << utl::print_map(sc.errorPages()) 			<< RESET << std::endl;
	o << HIPURPLE << std::setw(52) << "authorizedMethods : "<< HIGREEN << utl::print_vector(sc.authorizedMethods())	<< RESET << std::endl;
	o << HIPURPLE << std::setw(62) << "-----------------------------"	<< RESET << std::endl;

	return o;
}