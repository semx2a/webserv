#ifndef PARSER_HPP
# define PARSER_HPP

# include <fstream>
# include <iostream>
# include <sstream>

# include "CommonConfig.hpp"
# include "SpecConfig.hpp"

class Parser {

	public:
		Parser(std::string const& CommonConfig_file);
		~Parser();

		CommonConfig const &				getCommonConfig(void) const;
		std::vector<SpecConfig> const &	getSpecConfigs(void) const;

		void	parse(std::string const& CommonConfig_file);

	private:
		Parser();
		Parser(Parser const& rhs);
		Parser&	operator=(Parser const& rhs);

		CommonConfig				_CommonConfig;
		std::vector<SpecConfig>	_SpecConfigs;
};

#endif