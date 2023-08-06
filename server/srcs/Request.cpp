/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 18:48:05 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/06 19:04:31 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Request::Request(void) : _method(), _path(), _httpVersion(), _query(), _body(), _header() { };

Request::Request(Request const &src) {

	if (this != &src)
		*this = src;		
}

Request::~Request() { };

//::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMPARISON OPERATORS::

Request &	    Request::operator=(Request const & rhs) {
	
	if (this != &rhs) {
		
		this->setMethod(rhs.getMethod());
		this->setPath(rhs.getPath());
		this->setHTTP(rhs.getHTTP());
		this->setQuery(rhs.getQuery());
		this->setBody(rhs.getBody());
		this->setHeader(rhs.getHeader());
	}
	
	return *this;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

void	Request::setMethod(const std::string method) { this->_method = method; }

void	Request::setPath(const std::string path) { this->_path = path; }

void	Request::setHTTP(const std::string httpVersion) { this->_httpVersion = httpVersion; }

void	Request::setQuery(const std::string query) { this->_query = query; }

void	Request::setBody(const std::vector<char> body) { this->_body = body; }

void	Request::setHeader(const std::map<std::string, std::vector<std::string> > header) { this->_header = header; }

const std::string										Request::getMethod(void) const { return this->_method; }

const std::string										Request::getPath(void) const { return this->_path; }

const std::string										Request::getHTTP(void) const { return this->_httpVersion; }

const std::string										Request::getQuery(void) const { return this->_query; }

const std::vector<char>									Request::getBody(void) const { return this->_body; }

const std::map<std::string, std::vector<std::string> >	Request::getHeader(void) const { return this->_header; }

// ::::::::::::::::::::::::::::::::::::::::::::::: OUTPUT OPERATOR OVERLOADING::

std::ostream &	operator<<(std::ostream & o, Request const & r) {
	
	(void)r;
	return o << "hello Request" << std::endl;
};
