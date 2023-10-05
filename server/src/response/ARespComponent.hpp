#ifndef ARESPONSE_HPP
# define ARESPONSE_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <vector>

# include "macros.hpp"

class ARespComponent {

	public:
		ARespComponent();
		ARespComponent(std::string const &);
		ARespComponent(std::vector<char> const &);
		ARespComponent(ARespComponent const &);
		virtual ~ARespComponent();

		ARespComponent &		operator=(ARespComponent const &);
		
		virtual void					setMessage(std::string const &);
		virtual void					setMessage(std::vector<char> const &);
		virtual void					setContentLength(size_t const &);
		
		virtual std::string const &		getMessage() const;
		virtual size_t					getContentLength() const;

		virtual void	build() = 0;
	
	protected:
		std::string _message;
		size_t		_contentLength;
	
	private:
};

#endif