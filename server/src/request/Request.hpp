#ifndef REQUEST_HPP
# define REQUEST_HPP

//# define DEBUG_REQUEST

# include <string>
# include <iostream>
# include <map>
# include <vector>
# include <sstream>
# include <algorithm>

# include "Epoll.hpp"
# include "print.hpp"
# include "macros.hpp"

typedef std::map<std::string, std::string>	t_headers;

class Request {
	
	public:

		Request(void);
		Request(Request const &src);
		~Request(void);

		Request &	operator=(Request const &rhs);

		//GET/SET
		void	setMethod(const std::string);
		void	setTarget(const std::string);
		void	setQuery(const std::string);
		void	setVersion(const std::string);
		void	setHeaders(const t_headers);
		void	setBody(const std::vector<char>);
		
		std::string const &			getMethod(void) const;
		std::string const &			getTarget(void) const;
		std::string const &			getQuery(void) const;
		std::string const &			getVersion(void) const;
		std::vector<char> const &	getBody(void) const;
		std::string const &			getHeader(std::string const &) const;
		t_headers const &			getHeaders(void) const;

		//METHODS
		void	parser(std::vector<char> const& raw);
		
		//ERRORS
		class RequestLineException : public std::exception { 
			virtual const char* what() const throw() 
			{ return "Bad Request Line"; }};
		class HeadersException : public std::exception { 
			virtual const char* what() const throw() 
			{ return "Bad headers"; }};
		
		class BodyException : public std::exception { 
			virtual const char* what() const throw() 
			{ return "Bad body"; }};
	
	private:

		//VARIABLES	
		std::string			_method;
		std::string			_target;
		std::string			_query;
		std::string			_version;
		t_headers			_headers;
		std::vector<char>	_body;

		//PRIVATE METHODS
		void						_parseRequestLine(std::istringstream&);
		void						_parseHeaders(std::istringstream&);
		void						_parseBody(std::vector<char> const&);

		std::vector<std::string>	_tokenize(const std::string, char);
		std::string 				_trim(const std::string&);
		int 						_find_last_occurrence(const std::vector<char>& haystack, const std::string& needle);
};

std::ostream &	operator<<(std::ostream & o, Request const & r);

#endif
