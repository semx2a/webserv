#ifndef PARSER_HPP
# define PARSER_HPP

# include <cstdlib>

# include <fstream>
# include <iostream>
# include <sstream>
# include <string>

# include "ServerContext.hpp"

class ServerContext;

class Parser {

	public:
		Parser(Parser const& rhs);
		Parser&	operator=(Parser const& rhs);
		Parser(std::string const& conf_filename);
		~Parser();

		std::vector<ServerContext> const &	getServerContexts(void) const;
		std::string const &					getConfFileName(void) const;
		size_t								getLinesRead(void) const;

		void	parse();
		void	parseClientMaxBodySize(std::string const& line, ServerContext& specConfig);
		void	parseErrorPage(std::string const& line, ServerContext& specConfig);
		void	parseIndex(std::string const& line, ServerContext& specConfig);
		void	parseRoot(std::string const& line, ServerContext& specConfig);
		void	parseAutoindex(std::string const& line, ServerContext& specConfig);
		void	parseAuthorizedMethods(std::string const& line, ServerContext& specConfig);
		void	parseLocation(std::stringstream& stream, ServerContext& specConfig);
		void	parseListen(std::string const& line, ServerContext& specConfig);
		void	parseServerName(std::string const& line, ServerContext& serverContext);
		void	isValidIPv4(const std::string& str);
		void	isValidIPv6(const std::string& str);


	private:
		Parser();

		size_t						_linesRead;
		std::string		 			_confFilename;
		std::vector<ServerContext>	_serverContexts;

		void	parseServerContext(std::stringstream& stream);
	
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