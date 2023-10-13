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


class ServerContext;
class LocationContext;

class Parser {

	public:
		Parser(Parser const&);
		Parser&	operator=(Parser const&);
		Parser(std::string const&);
		~Parser();

		std::vector<ServerContext> const&	serverContexts(void) const;
		std::string const&					confFileName(void) const;
		size_t								linesRead(void) const;

		void	parse();

	private:
		Parser();

		size_t						_linesRead;
		int							_brackets;
		std::string		 			_confFilename;
		std::vector<ServerContext>	_serversContexts;

		template <typename Context>
		void	parseServerContext(std::stringstream&, Context&);
		
		template <typename Context>
		void	parseLocationContext(std::stringstream&, Context&);
		
		template <typename Context>
		struct DirectiveMap {
			typedef std::map<std::string, void(Parser::*)(std::string const&, Context&)> type;
		};
		
		DirectiveMap<ServerContext>::type initializeServerDirectives();
		DirectiveMap<LocationContext>::type initializeLocationDirectives();


		template <typename Context>
		void	parseAutoindex(std::string const&, Context&);
		template <typename Context>
		void 	parseMaxBodySize(const std::string&, Context&);
		template <typename Context>
		void	parseAlias(std::string const&, Context&);
		template <typename Context>
		void	parseRoot(std::string const&, Context&);
		template <typename Context>
		void	parseUploadFolder(std::string const&, Context&);
		template <typename Context>
		void	parseListen(std::string const&, Context&);
		template <typename Context>
		void	parseErrorPage(std::string const&, Context&);
		template <typename Context>
		void	parseIndex(std::string const&, Context&);
		template <typename Context>
		void	parseAuthorizedMethods(const std::string&, Context&);
		template <typename Context>
		void	parseServerName(std::string const&, Context&);

		// UTILS
		bool	isDirective(std::string const&) const;
		bool	isCommentOrEmptyLine(std::string const&) const;
		bool	isEndOfScope(std::string const&);
		void	isValidIPv4(std::string const&) const;
		void	buildAndThrowParamError(std::string const&) const;
		void	searchIfCgi(LocationContext&, std::string&);
		void	checkIfPortDoublons();

	
	public:
		class Error : public std::exception {
			public:
				Error(std::string);
				virtual ~Error() throw();
				virtual const char* what() const throw();
			protected:
				std::string _msg;
		};

		class InvalidParam : public Error {
			public:
				InvalidParam(std::string const&, Parser const&);
				virtual ~InvalidParam() throw();
		};

};

#include "Parser.tpp"

#endif