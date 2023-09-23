#ifndef LOCATION_CONTEXT_HPP
# define LOCATION_CONTEXT_HPP

#include "print.hpp"

class LocationContext {

	public:
		LocationContext();
		LocationContext(LocationContext const&);
		LocationContext& operator=(LocationContext const& rhs);
		~LocationContext();

		bool								getCgi() const;
		size_t								getMaxBodySize() const;
		std::string const&					getAlias() const;
		std::string const&					getRoot() const;
		std::map<int, std::string> const&	getErrorPages() const;
		std::vector<std::string> const&		getIndex() const;
		std::vector<std::string> const&		getAuthorizedMethods() const;

		void	setCgi(bool cgi);
		void	setMaxBodySize(size_t size);
		void	setAlias(std::string const& alias);
		void	setRoot(std::string const& root);
		void	addErrorPage(int code, std::string const& path);
		void	setErrorPages(std::map<int, std::string> const& errorPages);
		void	addIndex(std::string const& index);
		void	setIndex(std::vector<std::string> const& index);
		void	setAuthorizedMethods(std::vector<std::string> const& authorizedMethods);

	private:
		bool						_cgi;
		size_t						_maxBodySize;
		std::string					_alias;
		std::string					_root;
		std::map<int, std::string>	_errorPages;
		std::vector<std::string>	_index;
		std::vector<std::string>	_authorizedMethods;
};

std::ostream &	operator<<(std::ostream & o, LocationContext const & cc);

#endif

//RAJOUTER ALIAS

/*IDEA : make templates func parser for common variables :
									- cgi
									- maxBodySize
									- root
									- errorPages
									- authorizedMethods
									- index
that would take an unknown class (ServerContext or LocationContext, parse, and call the setter)
*/