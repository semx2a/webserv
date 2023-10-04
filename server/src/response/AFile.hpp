#ifndef AFILE_HPP
# define AFILE_HPP

# include <iostream>
# include <string>
# include <fstream>
# include <sstream>
# include <map>

# include "utl.hpp"

typedef std::map<std::string, std::string> t_lexicon;

class AFile {
	public:
		AFile();
		AFile(std::string const&);
		AFile(AFile const&);
		virtual ~AFile();

		AFile& operator=(AFile const&);

		std::string const&	fileName() const;
		t_lexicon const&	lexicon() const;

		void	setFileName(std::string const&);
		void	setLexicon(t_lexicon const&);

	protected:
		t_lexicon			_lexicon;
		std::string			_fileName;

		virtual t_lexicon	_initFile(std::string const&);
};

#endif