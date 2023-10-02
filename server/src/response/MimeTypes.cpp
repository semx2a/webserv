#include "MimeTypes.hpp"

MimeTypes::MimeTypes() : AFile() {}

MimeTypes::MimeTypes(std::string const & path) : AFile(path) {}

MimeTypes::~MimeTypes() {}

std::string const &	MimeTypes::extension(std::string const & extension) { return AFile::_lexicon.find(extension)->second; }