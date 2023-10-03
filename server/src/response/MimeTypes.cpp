#include "MimeTypes.hpp"

MimeTypes::MimeTypes() : AFile() {
	initFile("../neoserv/mime.types");
}

MimeTypes::~MimeTypes() {}

std::string const&	MimeTypes::getMimeType(std::string const& extension) const {

	std::map<std::string, std::string>::const_iterator it = this->lexicon().find(extension);

	if (it == this->lexicon().end())
		return this->lexicon().find("default")->second;
	return it->second;
}

t_lexicon	MimeTypes::initFile(std::string const& fileName) {

    std::ifstream file(fileName.c_str());

    if (!file) {
		throw std::runtime_error("Could not open " + fileName + " file");
    }

    std::map<std::string, std::string> mimeMap;
    std::string line;

    while (std::getline(file, line)) {
		std::cout << "Lecture de la ligne: " << line << std::endl; // Afficher chaque ligne lue

        if (line.empty() || line.find_first_of("{}") != std::string::npos) {
            continue; // Skip blank lines or lines that start with spaces/tabs
        }

		std::stringstream sstream(line);
		std::string value;
		std::string	key;

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
	return mimeMap;
}