/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 18:48:05 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/14 15:17:50 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Request::Request(void) : _method(), _target(), _query(), _version(), _headers(), _body() {}

Request::Request(Request const &src)
{
	if (this != &src)
		*this = src;
}

Request::Request(std::string const str)
{
	this->parser(str);
}

Request::~Request() {};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMPARISON OPERATORS::

Request &Request::operator=(Request const &rhs)
{

	if (this != &rhs)
	{
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

void Request::setMethod(const std::string method) { this->_method = method; }

void Request::setTarget(const std::string target) { this->_target = target; }

void Request::setVersion(const std::string version) { this->_version = version; }

void Request::setQuery(const std::string query) { this->_query = query; }

void Request::setBody(const std::vector<char> body) { this->_body = body; }

void Request::setHeaders(const std::map<std::string, std::vector<std::string> > headers) { this->_headers = headers; }

const std::string Request::getMethod(void) const { return this->_method; }

const std::string Request::getTarget(void) const { return this->_target; }

const std::string Request::getVersion(void) const { return this->_version; }

const std::string Request::getQuery(void) const { return this->_query; }

const std::vector<char> Request::getBody(void) const { return this->_body; }

const std::map<std::string, std::vector<std::string> > Request::getHeaders(void) const { return this->_headers; }

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::: MEMBER FUNCTIONS::

void Request::_parseBody(std::istringstream &stream) {
	
	std::string body;
	while (std::getline(stream, body, '\0') && !body.empty()) {

		for (std::string::iterator it = body.begin(); it != body.end(); it++) 
			this->_body.push_back(*it);
	}
}

void Request::_parseHeaders(std::istringstream &stream) {

	std::string line;
	std::vector<std::string> headers;

	while (std::getline(stream, line) && !line.empty()) {
		
		if (line.size() > 2)
			headers.push_back(line);
	}

	if (!this->_headers.empty())
		this->_headers.clear();

	for (std::vector<std::string>::iterator it = headers.begin(); it != headers.end(); ++it) {

		std::string	key;
		std::string values;
		size_t		pos;

		pos = 0;
		key.erase();
		values.erase();
		pos = it->find(":");
		key = it->substr(0, pos);
		values = it->substr(pos + 2, it->size());

		this->_headers[key].push_back(values);
		//tokenize values with ',' 
	}
}

void Request::_parseRequestLine(std::istringstream &stream) {
	
	stream >> this->_method >> this->_target >> this->_version;

	if (this->_method.empty() || this->_target.empty() || this->_version.emtpy())
		throw std::exception 
}

void Request::parser(std::string const str)
{
	std::istringstream iss(str);

	this->parseRequestLine(iss);
	this->parseHeaders(iss);
	if (!iss.eof())
		this->parseBody(iss);

	std::cout << *this << std::endl;

}

// ::::::::::::::::::::::::::::::::::::::::::::::: OUTPUT OPERATOR OVERLOADING::

std::ostream &operator<<(std::ostream &o, Request const &r)
{

	o << "  " << printLine(60, ":") << std::endl << std::endl;
	o << "method: " 	<< r.getMethod() << std::endl;
	o << "target: "		<< r.getTarget() << std::endl;
	o << "query: "		<< r.getQuery() << std::endl;
	o << "version: "	<< r.getVersion() << std::endl;
	o << "headers: "	<< std::endl;
	printMap(r.getHeaders());
	o << "body: " 		<< std::endl;
	printVector(r.getBody());

	return o;
};
