#ifndef HEADERS_HPP
# define HEADERS_HPP

# include "ARespComponent.hpp"
# include "ResponseContext.hpp"
# include "MimeTypes.hpp"

class Headers : public ARespComponent {
	
	public:
		Headers(size_t);
		Headers(std::string const&, size_t, std::string const&, ResponseContext const&, std::string const&);
		Headers(Headers const&);
		~Headers();
	
		Headers &	operator=(Headers const&);

		size_t		contentLength() const;
		void		setContentLength(size_t);

		void 		build();
		void		build(std::string const&, size_t, std::string const&, ResponseContext const&, std::string const&);

	private:
		Headers();
		size_t		_contentLength;
		MimeTypes	_mimeTypes;

		std::string	_findExtension(std::string const& path);
};

#endif