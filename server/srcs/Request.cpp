/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 18:48:05 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/08 19:22:47 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Request::Request(void) : _method(), _target(), _version(), _query(), _body(), _headers() { }

Request::Request(Request const &src) {

	if (this != &src)
		*this = src;		
}

Request::Request(std::string const str) {

	this->parse(str);
}

Request::~Request() { };

//::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMPARISON OPERATORS::

Request &	    Request::operator=(Request const & rhs) {
	
	if (this != &rhs) {
		
		this->setMethod(rhs.getMethod());
		this->setTarget(rhs.getTarget());
		this->setVersion(rhs.getVersion());
		this->setQuery(rhs.getQuery());
		this->setBody(rhs.getBody());
		this->setHeaders(rhs.getHeaders());
	}
	
	return *this;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

void	Request::setMethod(const std::string method) { this->_method = method; }

void	Request::setTarget(const std::string target) { this->_target = target; }

void	Request::setVersion(const std::string version) { this->_version = version; }

void	Request::setQuery(const std::string query) { this->_query = query; }

void	Request::setBody(const std::vector<char> body) { this->_body = body; }

void	Request::setHeaders(const std::map<std::string, std::vector<std::string> > headers) { this->_headers = headers; }


const std::string	Request::getMethod(void) const { return this->_method; }

const std::string	Request::getTarget(void) const { return this->_target; }

const std::string	Request::getVersion(void) const { return this->_version; }

const std::string	Request::getQuery(void) const { return this->_query; }

const std::vector<char>									Request::getBody(void) const { return this->_body; }

const std::map<std::string, std::vector<std::string> >	Request::getHeaders(void) const { return this->_headers; }

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::: MEMBER FUNCTIONS::

void			Request::parse(std::string const str) {
	
	(void)str;
}

// ::::::::::::::::::::::::::::::::::::::::::::::: OUTPUT OPERATOR OVERLOADING::

std::ostream &	operator<<(std::ostream & o, Request const & r) {
	
	o << r.getMethod() << std::endl;
	o << r.getTarget() << std::endl;
	o << r.getVersion() << std::endl;
	o << r.getQuery() << std::endl;
//	o << r.getBody() << std::endl;
//	o << r.getHeaders() << std::endl;
	
	return o;
};
