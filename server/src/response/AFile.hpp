#ifndef AFILE_HPP
# define AFILE_HPP

# include <iostream>
# include <string>
# include <fstream>
# include <sstream>
# include <map>

typedef std::map<std::string, std::string> t_lexicon;

class AFile {
	public:
		AFile();
		AFile(std::string const&);
		AFile(AFile const&);
		virtual ~AFile();

		AFile& operator=(AFile const&);

		t_lexicon const&	getLexicon() const;
		std::string const&	getFileName() const;

		void	setFileName(std::string const&);
		void	setLexicon(std::string const&);

	protected:
		t_lexicon	_lexicon;
		std::string	_fileName;

		std::string _trim(std::string const&);
		t_lexicon	_initFile(std::string const&);
};

#endif