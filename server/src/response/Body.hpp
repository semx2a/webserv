#ifndef BODY_HPP
# define BODY_HPP

# include "ARespComponent.hpp"

class Body : public ARespComponent {
	
	public:
		Body();
		Body(std::string const &);
		Body(Body const & src);
		~Body();
	
		Body &		operator=(Body const & rhs);

		void		build();
};

#endif