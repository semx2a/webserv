#ifndef ARESPONSE_HPP
# define ARESPONSE_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <vector>

# include "macros.hpp"

class ARespComponent {

	public:
		ARespComponent(void);
		ARespComponent(std::string const &);
		ARespComponent(std::vector<char> const &);
		ARespComponent(ARespComponent const &);
		virtual ~ARespComponent();

		ARespComponent &		operator=(ARespComponent const &);
		
		virtual void					setContent(std::string const &);
		virtual void					setContent(std::vector<char> const &);
		
		virtual std::string const &		getContent(void) const;

		virtual void	build(void) = 0;
	
	protected:
		std::string _content;
	
	private:
};

#endif