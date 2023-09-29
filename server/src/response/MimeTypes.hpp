#ifndef MIMETYPES_HPP
# define MIMETYPES_HPP

# include "AFile.hpp"

class MimeTypes : public AFile {
	public:
		MimeTypes();
		MimeTypes(std::string const&);
		~MimeTypes();

		std::string const& getExtension(std::string const&) const;
};

#endif