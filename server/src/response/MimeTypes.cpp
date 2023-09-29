#include "MimeTypes.hpp"

MimeTypes::MimeTypes() : AFile() {}

MimeTypes::MimeTypes(std::string const & path) : AFile(path) {}

MimeTypes::~MimeTypes() {}

std::string const &	MimeTypes::getExtension(std::string const & extension) const { return this->getLexicon().find(extension)->second; }