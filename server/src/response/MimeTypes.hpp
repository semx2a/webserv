#ifndef MIMETYPES_HPP
# define MIMETYPES_HPP

# include "AFile.hpp"

class MimeTypes : public AFile {
	public:
		MimeTypes();
		MimeTypes(std::string const&);
		~MimeTypes();

		static std::string const& extension(std::string const&);
};

#endif