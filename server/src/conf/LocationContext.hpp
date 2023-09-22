#ifndef LOCATION_CONTEXT_HPP
# define LOCATION_CONTEXT_HPP

#include "print.hpp"

class LocationContext {

	public:
		LocationContext();
		LocationContext(LocationContext const&);
		LocationContext& operator=(LocationContext const& rhs);
		~LocationContext();

		size_t								getMaxBodySize() const;
		std::string const&					getAlias() const;
		std::string const&					getRoot() const;
		std::map<int, std::string> const&	getErrorPages() const;
		std::vector<std::string> const&		getAuthorizedMethods() const;

		void	setMaxBodySize(size_t size);
		void	setAlias(std::string const& alias);
		void	setRoot(std::string const& root);
		void	setErrorPage(std::map<int, std::string> const& errorPages);
		void	setAuthorizedMethods(std::vector<std::string> const& authorizedMethods);

	private:
		size_t						_maxBodySize;
		std::string					_alias;
		std::string					_root;
		std::map<int, std::string>	_errorPages;
		std::vector<std::string>	_authorizedMethods;
};

std::ostream &	operator<<(std::ostream & o, LocationContext const & cc);

#endif

//IDEA : make templates func parser for common variables (root, maxBodySize, errorPages, authorizedMethods)
//that would take an unknown class (ServerContext or LocationContext, parse, and call the setter)