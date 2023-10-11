#include "LocationContext.hpp"

LocationContext::LocationContext() : 
AContext(), 
								_alias("") {}

LocationContext::LocationContext(LocationContext const& rhs) : AContext() { *this = rhs; }

LocationContext& LocationContext::operator=(LocationContext const& rhs) {

	if (this != &rhs) {
		
		AContext::operator=(rhs);
		this->_name = rhs.name();
		this->_alias = rhs.alias();
	}
	return *this;
}

LocationContext::~LocationContext() {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

std::string const&		LocationContext::name(void) const { return this->_name; }
std::string const&		LocationContext::alias(void) const { return this->_alias; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	LocationContext::setName(std::string const& name) { this->_name = name; }
void	LocationContext::setAlias(std::string const& alias) { this->_alias = alias; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void	LocationContext::setDefaults() {

	// NO DEFAULTS FOR LOCATION CONTEXT : SERVER DIRECTIVES WILL BE USED
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATOR OVERLOAD::

std::ostream &	operator<<(std::ostream & o, LocationContext const & sc) {

	o << HIPURPLE << "------ LocationContext ------"	<< RESET << std::endl;
	o << HIPURPLE << std::setw(52) << "name : " 			<< HIGREEN << sc.name() 								<< RESET << std::endl;
	o << HIPURPLE << std::setw(52) << "autoindex : " 		<< HIGREEN << sc.autoindex() 							<< RESET << std::endl;
	o << HIPURPLE << std::setw(52) << "maxBodySize : " 		<< HIGREEN << sc.maxBodySize() 							<< RESET << std::endl;
	o << HIPURPLE << std::setw(52) << "root : " 			<< HIGREEN << sc.root() 								<< RESET << std::endl;
	o << HIPURPLE << std::setw(52) << "errorPages : " 		<< HIGREEN << utl::print_map(sc.errorPages()) 			<< RESET << std::endl;
	o << HIPURPLE << std::setw(52) << "index : " 			<< HIGREEN << utl::print_vector(sc.index()) 			<< RESET << std::endl;
	o << HIPURPLE << std::setw(52) << "authorizedMethods : "<< HIGREEN << utl::print_vector(sc.authorizedMethods())	<< RESET << std::endl;
	o << HIPURPLE << std::setw(52) << "uploadFolder : " 	<< HIGREEN << sc.uploadFolder() 						<< RESET << std::endl;

	o << HIPURPLE << std::setw(52) << "alias : " 			<< HIGREEN << sc.alias() 								<< RESET << std::endl;
	o << HIPURPLE << std::setw(62) << "-----------------------------"	<< RESET << std::endl;

	return o;
}