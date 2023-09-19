/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 18:48:05 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/17 17:51:32 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Request::Request(void) : _method(), _target(), _query(), _version(), _headers(), _body(), _isFirstLine(0), _isHeader(0), _isBody(0), _isQuery(0) {}

Request::Request(Request const &src)
{
	if (this != &src)
		*this = src;
}

Request::Request(std::vector<char> const str)
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

int Request::_find_last_occurrence(const std::vector<char>& haystack, const std::string& needle) {
    // Convert the vector of characters to a string
    std::string haystack_str(haystack.begin(), haystack.end());

    // Use rfind to find the last occurrence of the needle
    size_t pos = haystack_str.rfind(needle);

    // If the needle is not found, return -1
    if(pos == std::string::npos) {
        return -1;
    }

    // Otherwise, return the position of the last character of the needle
    return pos + needle.size() - 1;
}


void Request::_parseBody(std::vector<char>& str_vec) {
	
	int pos = this->_find_last_occurrence(str_vec, "\r\n\r\n");
	
	this->_body.assign(str_vec.begin() + (pos + 1), str_vec.end());
}

std::string Request::_trim(const std::string& str)
{
    const std::string ws = " \n\r\t\f\v";
    
    size_t start = str.find_first_not_of(ws);
    if (start == std::string::npos)
        return ""; // no content except whitespace

    size_t end = str.find_last_not_of(ws);
    
    return str.substr(start, end-start+1);
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
	std::vector<std::string>	headers_vec;

	if (!this->_headers.empty())
		this->_headers.clear();
	
	while (std::getline(stream, line, '\r')) {

		line = this->_trim(line);
		if (!line.empty())
			headers_vec.push_back(line);
	}
	
	for (std::vector<std::string>::iterator it = headers_vec.begin(); it != headers_vec.end(); it++)
	{
		std::istringstream	hd_line(*it);
		std::string			hd_key;
		std::string 		hd_values;
		
		hd_key.erase(); 	
		hd_values.erase();
		std::getline(hd_line, hd_key, ':');
		std::getline(hd_line, hd_values);

		if (hd_key.empty())
			throw HeadersException();
		if (hd_values.empty())
			throw HeadersException();
		
		hd_values = this->_trim(hd_values);
 		this->_headers[hd_key].push_back(hd_values);
		//this->_headers[hd_key] = this->_tokenize(hd_values, ',');
	}
	
	if (this->getHeaders().empty())
		throw HeadersException();
	else
		this->setIsHeader(true);
}

void Request::_parseRequestLine(std::istringstream &stream) {
	
	stream >> this->_method >> this->_target >> this->_version;
	
	if (this->_method.empty() || this->_target.empty() || this->_version.empty())
		throw RequestLineException();
	else
		this->setIsFirstLine(true);
}

void Request::parser(std::vector<char> str_vec) {
	
	std::string			str(str_vec.begin(), str_vec.end());
	std::istringstream	stream(str);

	this->_parseRequestLine(stream);
	this->_parseHeaders(stream);
	if (!stream.eof())
		this->_parseBody(str_vec);

	std::cout << *this << std::endl;

}

// ::::::::::::::::::::::::::::::::::::::::::::::: OUTPUT OPERATOR OVERLOADING::

std::ostream &operator<<(std::ostream &o, Request const &r) {

	if (r.getIsFirstLine()) {
		o << "  " << str_of(60, ":") << std::endl << std::endl;
		o << "method: " 	<< r.getMethod() << std::endl;
		o << "target: "		<< r.getTarget() << std::endl;
		o << "version: "	<< r.getVersion() << std::endl;
	}
	if (r.getIsQuery())
		o << "query: "		<< r.getQuery() << std::endl;
		
	if (r.getIsHeader()) {
		o << "headers: "	<< std::endl;
		print_map_of_vectors(r.getHeaders());
	}
	
	if (r.getIsBody()) {
		o << "body: " 		<< std::endl;
		print_vector(r.getBody());
	}

	return o;
}
