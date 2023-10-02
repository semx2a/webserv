#include "LocationContext.hpp"

LocationContext::LocationContext() : AContext(), _alias(""), _isPyCgi(false) {}

LocationContext::LocationContext(LocationContext const& rhs) : AContext() { *this = rhs; }

LocationContext& LocationContext::operator=(LocationContext const& rhs) {

	if (this != &rhs) {
		
		AContext::operator=(rhs);
		this->_alias = rhs.alias();
		this->_isPyCgi = rhs.isPyCgi();
	}
	return *this;
}

LocationContext::~LocationContext() {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

std::string const&		LocationContext::alias(void) const { return this->_alias; }
bool					LocationContext::isPyCgi(void) const { return this->_isPyCgi; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	LocationContext::setAlias(std::string const& alias) { this->_alias = alias; }
void	LocationContext::setIsPyCgi(bool isPyCgi) { this->_isPyCgi = isPyCgi; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void	LocationContext::setDefaults() {

	AContext::setDefaults();
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATOR OVERLOAD::

std::ostream &	operator<<(std::ostream & o, LocationContext const & sc) {

	o << HIPURPLE << "------ LocationContext ------"	<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "maxBodySize : " 		<< HIGREEN << sc.maxBodySize() 						<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "alias : " 			<< HIGREEN << sc.alias() 							<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "root : " 			<< HIGREEN << sc.root() 							<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "errorPages : " 		<< HIGREEN << print_map(sc.errorPages()) 			<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(52) << "authorizedMethods : "<< HIGREEN << print_vector(sc.authorizedMethods())	<< NO_COLOR << std::endl;
	o << HIPURPLE << std::setw(62) << "-----------------------------"	<< NO_COLOR << std::endl;

	return o;
}