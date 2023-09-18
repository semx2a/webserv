#ifndef PARSER_HPP
# define PARSER_HPP

# include <fstream>
# include <iostream>
# include <sstream>
# include <string>

# include "CommonConfig.hpp"
# include "SpecConfig.hpp"

class Parser {

	public:
		Parser(std::string const& conf_filename);
		~Parser();

		CommonConfig const &				getCommonConfig(void) const;
		std::vector<SpecConfig> const &		getSpecConfigs(void) const;

		void	parse(std::string const& conf_filename);

		class Error {
			public:
				Error(std::string const& msg);
				~Error() throw();
				const char* what() const throw();
			private:
				std::string _msg;
		};

	private:
		Parser();
		Parser(Parser const& rhs);
		Parser&	operator=(Parser const& rhs);

		CommonConfig				_CommonConfig;
		std::vector<SpecConfig>		_SpecConfigs;
};

#endif