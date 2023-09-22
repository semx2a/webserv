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

	private:
		Parser();

		size_t						_linesRead;
		std::string		 			_confFilename;
		std::vector<ServerContext>	_serverContexts;

		void	parseServerContext(std::stringstream& stream);
		void	parseServerLocationContext(std::stringstream& stream, ServerContext& serverContext);

		typedef void (Parser::*ParserFunction)(std::string const&, ServerContext&);
    	typedef std::pair<std::string, Parser::ParserFunction> DirectivePair;
		typedef std::map<std::string, Parser::ParserFunction> DirectiveMap;

		void	parseAutoindex(std::string const& line, ServerContext& serverContext);
		template<typename Context>
		void	parseCgi(std::string const& line, Context& context);
		template<typename Context>
		void 	parseMaxBodySize(const std::string& line, Context& context);
		template<typename Context>
		void	parseRoot(std::string const& line, Context& context);
		void	parseListen(std::string const& line, ServerContext& serverContext);
		template<typename Context>
		void	parseErrorPage(std::string const& line, Context& context);
		void	parseIndex(std::string const& line, ServerContext& serverContext);
		template<typename Context>
		void	parseAuthorizedMethods(const std::string& line, Context& context);
		void	parseServerName(std::string const& line, ServerContext& serverContext);

		// UTILS
		bool	isDirective(std::string const& line) const;
		bool	isCommentOrEmptyLine(std::string const& line) const;
		bool	isEndOfScope(std::string const& line) const;
		void	isValidIPv4(std::string const& str) const;
		void	isValidIPv6(std::string const& str) const;
		void 	trimAndReplaceWhitespaces(std::string& input);
		void	buildAndThrowParamError(std::string const& line) const;

	
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