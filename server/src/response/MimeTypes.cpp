#include "MimeTypes.hpp"

MimeTypes::MimeTypes() : AFile("../neoserv/mime.types") {}

MimeTypes::~MimeTypes() {}

std::string const&	MimeTypes::getMimeType(std::string const& extension) const {

	std::map<std::string, std::string>::const_iterator it = this->lexicon().find(extension);

	if (it == this->lexicon().end())
		return this->lexicon().find("default")->second;
	return it->second;
}