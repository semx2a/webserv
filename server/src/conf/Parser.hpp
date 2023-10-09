#ifndef PARSER_HPP
# define PARSER_HPP

# include <cstdlib>

# include <fstream>
# include <iostream>
# include <set>
# include <sstream>
# include <string>
# include <typeinfo>

# include "ServerContext.hpp"
# include "LocationContext.hpp"

//# define DEBUG_PARSER

class ServerContext;
class LocationContext;

class Parser {

	public:
		Parser(Parser const& rhs);
		Parser&	operator=(Parser const& rhs);
		Parser(std::string const& conf_filename);
		~Parser();

		std::vector<ServerContext> const &	serverContexts(void) const;
		std::string const &					confFileName(void) const;
		size_t								linesRead(void) const;

		void	parse();

	private:
		Parser();

		size_t						_linesRead;
		std::string		 			_confFilename;
		std::vector<ServerContext>	_serversContexts;

		template <typename Context>
		void	parseServerContext(std::stringstream& stream, Context& context);
		
		template <typename Context>
		void	parseLocationContext(std::stringstream& stream, Context& context);
		
		template <typename Context>
		struct DirectiveMap {
			typedef std::map<std::string, void(Parser::*)(std::string const&, Context&)> type;
		};
		
		DirectiveMap<ServerContext>::type initializeServerDirectives();
		DirectiveMap<LocationContext>::type initializeLocationDirectives();


		template <typename Context>
		void	parseAutoindex(std::string const& line, Context&);
		template <typename Context>
		void 	parseMaxBodySize(const std::string& line, Context&);
		template <typename Context>
		void	parseAlias(std::string const& line, Context& context);
		template <typename Context>
		void	parseRoot(std::string const& line, Context&);
		template <typename Context>
		void	parseListen(std::string const& line, Context&);
		template <typename Context>
		void	parseErrorPage(std::string const& line, Context&);
		template <typename Context>
		void	parseIndex(std::string const& line, Context&);
		template <typename Context>
		void	parseAuthorizedMethods(const std::string& line, Context&);
		template <typename Context>
		void	parseServerName(std::string const& line, Context&);

		// UTILS
		bool	isDirective(std::string const& line) const;
		bool	isCommentOrEmptyLine(std::string const& line) const;
		bool	isEndOfScope(std::string const& line) const;
		void	isValidIPv4(std::string const& str) const;
		void	buildAndThrowParamError(std::string const& line) const;
		void	searchIfCgi(LocationContext&, std::string&);
		void	checkIfPortDoublons();

	
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

#include "Parser.tpp"

#endif