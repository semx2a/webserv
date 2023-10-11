#include "MimeTypes.hpp"

MimeTypes::MimeTypes() : AFile() {
	
	this->setLexicon(this->initFile("../neoserv/mime.types"));
}
MimeTypes::~MimeTypes() {}

std::string const&	MimeTypes::getMimeType(std::string const& extension) const {

	t_lexicon::const_iterator it = this->lexicon().find(extension);

	if (it == this->lexicon().end())
		return this->lexicon().find("txt")->second;
	return it->second;
}

t_lexicon	MimeTypes::initFile(std::string const& fileName) {

    std::ifstream file(fileName.c_str());

    if (!file.is_open()) {
		throw std::runtime_error("Could not open " + fileName + " file");
    }

    t_lexicon mimeMap;
    std::string line;

    while (std::getline(file, line) && !file.eof()) {

        if (line.empty() || line.find_first_of("{}") != std::string::npos)
            continue; // Skip blank lines or lines that start with spaces/tabs

		std::stringstream sstream(line);
		std::string value;
		std::string	key;

		key.clear();
		value.clear();

		sstream >> value;

		while (sstream >> key) {
		
			if (key.find(";") != std::string::npos) {
				key = key.substr(0, key.find_first_of(";"));
				mimeMap[key] = value;
				break;
			}
			mimeMap[key] = value;
		}
    }
	file.close();
	return mimeMap;
}