#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <iostream>
# include <map>
# include <vector>
# include <sstream>
# include <algorithm>

# include "Epoll.hpp"
# include "utl.hpp"
# include "macros.hpp"
# include "HttpStatus.hpp"

typedef std::map<std::string, std::string>	t_headers;

class Request {
	
	public:

		Request();
		Request(Request const &src);
		~Request();

		Request &	operator=(Request const &rhs);

		//GET/SET
		void	setMethod(const std::string);
		void	setTarget(const std::string);
		void	setQuery(const std::string);
		void	setVersion(const std::string);
		void	setHeaders(const t_headers);
		void	setBody(const std::vector<char>);
		void	setBoundary(const std::string);
		
		std::string const &			method() const;
		std::string const &			target() const;
		std::string const &			query() const;
		std::string const &			version() const;
		std::vector<char> const &	body() const;
		std::string const &			header(std::string const &) const;
		t_headers const &			headers() const;
		std::string const &			boundary() const;

		//METHODS
		void	parser(std::vector<char> const& raw, std::string const& boundary);
		void	parseHeaders(std::istringstream&);
		void	parseBody(std::vector<char> const&);
		
	private:

		//VARIABLES	
		std::string			_method;
		std::string			_target;
		std::string			_query;
		std::string			_version;
		t_headers			_headers;
		std::vector<char>	_body;
		std::string			_boundary;

		//PRIVATE METHODS
		void						_parseRequestLine(std::istringstream&);
};

std::ostream &	operator<<(std::ostream & o, Request const & r);

#endif
