#ifndef LOCATION_CONTEXT_HPP
# define LOCATION_CONTEXT_HPP

#include "AContext.hpp"

class LocationContext : public AContext {

	public:
		LocationContext(void);
		LocationContext(LocationContext const&);
		LocationContext& operator=(LocationContext const& rhs);
		~LocationContext();

	 	std::string const&	name(void) const;
		std::string const&	alias(void) const;

		void				setName(std::string const& name);
		void				setAlias(std::string const& alias);

		void				setDefaults(void);

	private:
	 	std::string		_name;
		std::string		_alias;
};

std::ostream &	operator<<(std::ostream & o, LocationContext const & cc);

#endif