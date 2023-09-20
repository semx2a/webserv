#ifndef PARSER_HPP
# define PARSER_HPP

# include <cstdlib>

# include <fstream>
# include <iostream>
# include <sstream>
# include <string>

# include "CommonConfig.hpp"
# include "SpecConfig.hpp"

class SpecConfig;

class Parser {

	public:
		Parser(Parser const& rhs);
		Parser&	operator=(Parser const& rhs);
		Parser(std::string const& conf_filename);
		~Parser();

		CommonConfig const &				getCommonConfig(void) const;
		std::vector<SpecConfig> const &		getSpecConfigs(void) const;
		std::string const &					getConfFileName(void) const;
		size_t								getLinesRead(void) const;

		void	parse();
		void	parseListen(std::string const& line, SpecConfig& specConfig);
		void	parseServerName(std::string const& line, SpecConfig& specConfig);
		void	isValidIPv4(const std::string& str);
		void	isValidIPv6(const std::string& str);


	private:
		Parser();

		size_t					_linesRead;
		std::string		 		_confFilename;
		CommonConfig			_commonConfig;
		std::vector<SpecConfig>	_specConfigs;

		void	parseSpecConfig(std::stringstream& stream);
	
	public:
		class Error : public std::exception {
			public:
				Error(std::string msg);
				virtual ~Error() throw();
				virtual const char* what() const throw();
			protected:
				std::string _msg;
		};

		class InvalidParam : public Error {
			public:
				InvalidParam(std::string const& err, Parser const& p);
				virtual ~InvalidParam() throw();
		};

};

#endif