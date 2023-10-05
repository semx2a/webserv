#ifndef HEADERS_HPP
# define HEADERS_HPP

# include "ARespComponent.hpp"
# include "ServerContext.hpp"

class Headers : public ARespComponent {
	
	public:
		Headers(size_t const&);
		Headers(std::string const&, size_t const&, ServerContext const&);
		Headers(Headers const&);
		~Headers();
	
		Headers &	operator=(Headers const&);

		void		build(std::string const&, size_t const&, ServerContext const&);

	private:
		Headers();
		void		build();
};

#endif