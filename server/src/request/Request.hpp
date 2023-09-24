/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 17:07:53 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/17 17:49:22 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <iostream>
# include <map>
# include <vector>
# include <sstream>
# include <algorithm>

# include "Epoll.hpp"
# include "print.hpp"
# include "macros.hpp"

class Request {
	
	public:

		Request(void);
		Request(Request const &src);
		Request(std::vector<char> const);
		~Request(void);

		Request &	operator=(Request const &rhs);

		//GET/SET
		void	setMethod(const std::string);
		void	setTarget(const std::string);
		void	setQuery(const std::string);
		void	setVersion(const std::string);
		void	setHeaders(const std::map<std::string, std::vector<std::string> >);
		void	setBody(const std::vector<char>);
		
		const std::string &			getMethod(void) const;
		const std::string &			getTarget(void) const;
		const std::string &			getQuery(void) const;
		const std::string &			getVersion(void) const;
		const std::vector<char> &	getBody(void) const;
		const std::map<std::string, std::vector<std::string> > &	getHeaders(void) const;

		//DB
		void	setIsFirstLine(const bool);
		void	setIsHeader(const bool);
		void	setIsBody(const bool);
		void	setIsQuery(const bool);
		bool	getIsFirstLine(void) const;
		bool	getIsHeader(void) const;
		bool	getIsBody(void) const;
		bool	getIsQuery(void) const;

		//METHODS
		void	parser(std::vector<char>);
		
		//ERRORS
		class RequestLineException : public std::exception { 
			virtual const char* what() const throw() 
			{ return "Bad Request LIne"; }};
		class HeadersException : public std::exception { 
			virtual const char* what() const throw() 
			{ return "Bad headers"; }};
		
		class BodyException : public std::exception { 
			virtual const char* what() const throw() 
			{ return "Bad body"; }};
	
	private:

		//VARIABLES	
		std::string												_method;
		std::string												_target;
		std::string												_query;
		std::string												_version;
		std::map<std::string, std::vector<std::string> >		_headers;
		std::vector<char>										_body;

		//DB
		bool													_isFirstLine;
		bool													_isHeader;
		bool													_isBody;
		bool													_isQuery;

		//PRIVATE METHODS
		void						_parseRequestLine(std::istringstream&);
		void						_parseHeaders(std::istringstream&);
		void						_parseBody(std::vector<char>&);

		std::vector<std::string>	_tokenize(const std::string, char);
		std::string 				_trim(const std::string&);
		int 						_find_last_occurrence(const std::vector<char>& haystack, const std::string& needle);
};

std::ostream &	operator<<(std::ostream & o, Request const & r);

#endif
