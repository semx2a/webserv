/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 18:48:05 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/16 17:47:29 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Request::Request(void) : _method(), _target(), _query(), _version(), _headers(), _body(), _isFirstLine(0), _isHeader(0), _isBody(0), _isQuery(0) {}

Request::Request(Request const &src)
{
	if (this != &src)
		*this = src;
}

Request::Request(std::string const str)
{
	this->parser(str);
}

Request::~Request() {}

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


const std::string & Request::getMethod(void) const { return this->_method; }

const std::string & Request::getTarget(void) const { return this->_target; }

const std::string & Request::getVersion(void) const { return this->_version; }

const std::string & Request::getQuery(void) const { return this->_query; }

const std::vector<char> & Request::getBody(void) const { return this->_body; }

const std::map<std::string, std::vector<std::string> > & Request::getHeaders(void) const { return this->_headers; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: DEBUG::

void Request::setIsFirstLine(const bool tf) { this->_isFirstLine = tf; }
void Request::setIsHeader(const bool tf) { this->_isHeader = tf; }
void Request::setIsBody(const bool tf) { this->_isBody = tf; }
void Request::setIsQuery(const bool tf) { this->_isQuery = tf; }

bool Request::getIsFirstLine(void) const { return this->_isFirstLine; }
bool Request::getIsHeader(void) const { return this->_isHeader; }
bool Request::getIsBody(void) const { return this->_isBody; }
bool Request::getIsQuery(void) const { return this->_isQuery; }

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::: MEMBER FUNCTIONS::

void Request::_parseBody(std::istringstream &stream) {
	
	std::string body;
	
	while (std::getline(stream, body, '\0') && !body.empty()) {

		for (std::string::iterator it = body.begin(); it != body.end(); it++) 
			this->_body.push_back(*it);
	}

}

std::vector<std::string> Request::_tokenize(const std::string str, char sep) {
	
	std::vector<std::string>	token_vector;
	std::string					token;
	std::istringstream 			str_stream(str);

	while (getline(str_stream, token, sep))
	{
		token_vector.push_back(token);
		if (token.empty())
			throw HeadersException();
	}
	
	return token_vector;
}

void Request::_parseHeaders(std::istringstream &stream) {

	std::string 				line;
	std::vector<std::string> headers;

	if (!this->_headers.empty())
		this->_headers.clear();
	
	while (std::getline(stream, line) && !line.empty()) {
		
		if (line.compare(0, 2, CRLF))
			headers.push_back(line);
	}

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

/* 		this->_headers[key].push_back(values); */
		this->_headers[key] = this->_tokenize(values, ',');
	}
	if (this->getHeaders().empty())
		throw HeadersException();
	else
		this->setIsHeader(true);

	line.clear();
}

void Request::_parseRequestLine(std::istringstream &stream) {
	
	stream >> this->_method >> this->_target >> this->_version;
	
	if (this->_method.empty() || this->_target.empty() || this->_version.empty())
		throw RequestLineException();
	else
		this->setIsFirstLine(true);
}

void Request::parser(std::string const str) {
	
	std::istringstream iss(str);

	this->_parseRequestLine(iss);
	this->_parseHeaders(iss);
	if (!iss.eof())
		this->_parseBody(iss);

	std::cout << *this << std::endl;

}

// ::::::::::::::::::::::::::::::::::::::::::::::: OUTPUT OPERATOR OVERLOADING::

std::ostream &operator<<(std::ostream &o, Request const &r) {

	if (r.getIsFirstLine()) {
		o << "  " << printLine(60, ":") << std::endl << std::endl;
		o << "method: " 	<< r.getMethod() << std::endl;
		o << "target: "		<< r.getTarget() << std::endl;
		o << "version: "	<< r.getVersion() << std::endl;
	}
	if (r.getIsQuery())
		o << "query: "		<< r.getQuery() << std::endl;
		
	if (r.getIsHeader()) {
		o << "headers: "	<< std::endl;
		printMap(r.getHeaders());
	}
	
	if (r.getIsBody()) {
		o << "body: " 		<< std::endl;
		printVector(r.getBody());
	}

	return o;
}
