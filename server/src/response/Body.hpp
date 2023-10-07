#ifndef BODY_HPP
# define BODY_HPP

# include "ARespComponent.hpp"

class Body : public ARespComponent {
	
	public:
		Body();
		Body(std::string const &);
		Body(std::vector<char> const &);
		Body(Body const & src);
		~Body();
	
		Body &		operator=(Body const & rhs);

		size_t		getContentLength() const;

		void		setContentLength(size_t contentLength);

		void		build();
		void		build(std::string const &);
		void		build(std::vector<char> const &);
	
	private:
		size_t		_contentLength;
};

#endif