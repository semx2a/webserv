#include "MimeTypes.hpp"

MimeTypes::MimeTypes() : AFile() {}

MimeTypes::MimeTypes(std::string const & path) : AFile(path) {}

MimeTypes::~MimeTypes() {}

std::string const &	MimeTypes::extension(std::string const & extension) const { return this->lexicon().find(extension)->second; }