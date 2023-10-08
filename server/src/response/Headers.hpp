#ifndef HEADERS_HPP
# define HEADERS_HPP

# include "ARespComponent.hpp"
# include "ResponseContext.hpp"

class Headers : public ARespComponent {
	
	public:
		Headers(size_t const&);
		Headers(std::string const&, size_t const&, ResponseContext const&);
		Headers(Headers const&);
		~Headers();
	
		Headers &	operator=(Headers const&);

		void				setContentLength(size_t);
		size_t				contentLength() const;

		void		build(std::string const&, size_t const&, ResponseContext const&);

	private:
		Headers();
		void		build();
		size_t		_contentLength;
};

#endif