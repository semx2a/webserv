#ifndef ARESPONSE_HPP
# define ARESPONSE_HPP

# include <iostream>
# include <string>
# include <sstream>

# include "macros.hpp"

class ARespComponent {

	public:
		ARespComponent();
		ARespComponent(ARespComponent const & src);
		virtual ~ARespComponent();

		ARespComponent &		operator=(ARespComponent const & rhs);

		void			setMessage(std::string const & message);
		std::string		getMessage() const;

		virtual void	build() = 0;
	
	protected:
		std::string _message;
};

#endif