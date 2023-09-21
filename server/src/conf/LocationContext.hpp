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
		std::vector<std::string> const&		getLimitExcept() const;

	private:
		size_t						_maxBodySize;
		std::string					_alias;
		std::string					_root;
		std::map<int, std::string>	_errorPages;
		std::vector<std::string>	_limitExcept;
};

#endif

//IDEA : make templates func parser for common variables (root, maxBodySize, errorPages, limitExcept)
//that would take an unknown class (ServerContext or LocationContext, parse, and call the setter)