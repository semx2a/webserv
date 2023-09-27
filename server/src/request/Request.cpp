/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 18:48:05 by seozcan           #+#    #+#             */
/*   Updated: 2023/09/27 16:38:01y seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Request::Request(void) : _method(), _target(), _query(), _version(), _headers(), _body() {}

Request::Request(Request const &src) {
	if (this != &src)
		*this = src;
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
void Request::setHeaders(const std::map<std::string, std::string> headers) { this->_headers = headers; }


std::string const & 		Request::getMethod(void) const { return this->_method; }
std::string const & 		Request::getTarget(void) const { return this->_target; }
std::string const & 		Request::getVersion(void) const { return this->_version; }
std::string const & 		Request::getQuery(void) const { return this->_query; }
std::vector<char> const & 	Request::getBody(void) const { return this->_body; }
std::string const & 		Request::getHeader(std::string const & key) const { return this->_headers.find(key)->second; }
t_headers const &		 	Request::getHeaders(void) const { return this->_headers; }

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::: MEMBER FUNCTIONS::

int Request::_find_last_occurrence(const std::vector<char> & haystack, const std::string& needle) {
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


void Request::_parseBody(std::vector<char> const& str_vec) {
	
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
 		this->_headers[hd_key] = hd_values;
		//this->_headers[hd_key] = this->_tokenize(hd_values, ',');
	}
	
	if (this->getHeaders().empty())
		throw HeadersException();
}

void Request::_parseRequestLine(std::istringstream &stream) {
	
	stream >> this->_method >> this->_target >> this->_version;

	if (this->_method.empty() || this->_target.empty() || this->_version.empty())
		throw RequestLineException();
}

void Request::parser(std::vector<char> const& str_vec) {
	
	std::string			str(str_vec.begin(), str_vec.end());
	std::istringstream	stream(str);

	if (str.empty())
		return ;
	this->_parseRequestLine(stream);
	this->_parseHeaders(stream);
	if (!stream.eof())
		this->_parseBody(str_vec);

	std::cout << *this << std::endl;

}

// ::::::::::::::::::::::::::::::::::::::::::::::: OUTPUT OPERATOR OVERLOADING::

std::ostream &operator<<(std::ostream &o, Request const &r) {

	o << "  " << str_of(60, ":") << std::endl << std::endl;
	o << "method: " 	<< r.getMethod() << std::endl;
	o << "target: "		<< r.getTarget() << std::endl;
	o << "version: "	<< r.getVersion() << std::endl;
	o << "query: "		<< r.getQuery() << std::endl;
	o << "headers: "	<< print_map(r.getHeaders()) << std::endl;
	o << "body: " 		<< print_vector(r.getBody()) << std::endl;

	return o;
}
