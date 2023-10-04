#include "AFile.hpp"


// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

AFile::AFile() {}

AFile::AFile(const std::string &filename) {

	this->setLexicon(this->_initFile(filename));
}

AFile::AFile(const AFile &rhs) {
	
	*this = rhs;
}

AFile::~AFile() {}

// :::::::::::::::::::::::::::::::::::::::::::::::::::::: COMPARISON OPERATORS::

AFile &AFile::operator=(const AFile &rhs) {

	if (this != &rhs) {

		this->_lexicon = rhs.lexicon();
		this->_fileName = rhs.fileName();
	}
	return *this;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

std::string const& 	AFile::fileName() const { return this->_fileName; }
t_lexicon const& 	AFile::lexicon() const { return this->_lexicon; }

void	AFile::setFileName(std::string const& filename) { this->_fileName = filename; }
void	AFile::setLexicon(t_lexicon const & lexicon) { this->_lexicon = lexicon; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

t_lexicon AFile::_initFile(std::string const& filename) {

	std::ifstream	file(filename.c_str());

	if (!file.is_open())
		throw std::runtime_error("Could not open " +  filename + " file");

	t_lexicon		lexiconFile;
	std::string		line;
	
	while (std::getline(file, line) && !file.eof())
	{

		if (line.empty())
			continue;

		if (line.find_first_of("{}") != std::string::npos)
			continue;
		
		std::string			key;
		std::string			value;
		std::stringstream 	sstream(line);
		
		key.clear();
		value.clear();

		sstream >> key;
		std::getline(sstream, value, ';');
		value = utl::trim(value);
		lexiconFile[key] = value;
	}
	
	file.close();
	return (lexiconFile);
}
