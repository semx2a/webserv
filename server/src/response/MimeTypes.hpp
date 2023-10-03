#ifndef MIMETYPES_HPP
# define MIMETYPES_HPP

# include "AFile.hpp"

class MimeTypes : public AFile {
	public:
		MimeTypes();
		~MimeTypes();

		std::string const&	getMimeType(std::string const&) const;
		t_lexicon 			initFile(std::string const&);
};

#endif