/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 10:34:31 by seozcan           #+#    #+#             */
/*   Updated: 2023/09/27 13:09:55 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Response::Response(void) {
}

Response::Response(Response const& rhs) {
	if (this != &rhs)
		*this = rhs;
}

Response::~Response(void) {}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMPARISON OPERATORS::

Response& Response::operator=(Response const& rhs)
{
	if (this != &rhs)
	{
		this->_response = rhs._response;
	}
	return (*this);
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

std::map<int, std::string> const&	Response::getStatusCodes(void) const { return (this->_statusCodes); }

std::string const & 				Response::getStatusCode(int code) const { return (this->_statusCodes.find(code)->second); }

std::string const&					Response::getResponse(void) const {	return (this->_response); }

void	Response::setStatusLine(std::string const& statusLine) { this->_statusLine = statusLine; }

void	Response::setStatusCodes(void) {

	this->_statusCodes[1]	= "Informational - Request received, continuing process";
	this->_statusCodes[100] = "Continue";
	this->_statusCodes[101] = "Switching Protocols";

	this->_statusCodes[2]	= "Success - The action was successfully received, understood, and accepted";
	this->_statusCodes[200] = "OK";
	this->_statusCodes[201] = "Created";
	this->_statusCodes[202] = "Accepted";
	this->_statusCodes[203] = "Non-Authoritative Information";
	this->_statusCodes[204] = "No Content";
	this->_statusCodes[205] = "Reset Content";
	this->_statusCodes[206] = "Partial Content";

	this->_statusCodes[3]	= "Redirection - Further action must be taken in order to complete the request";
	this->_statusCodes[300] = "Multiple Choices";
	this->_statusCodes[301] = "Moved Permanently";
	this->_statusCodes[302] = "Found";
	this->_statusCodes[303] = "See Other";
	this->_statusCodes[304] = "Not Modified";
	this->_statusCodes[305] = "Use Proxy";
	this->_statusCodes[307] = "Temporary Redirect";

	this->_statusCodes[4]	= "Client Error - The request contains bad syntax or cannot be fulfilled";
	this->_statusCodes[400] = "Bad Request";
	this->_statusCodes[401] = "Unauthorized";
	this->_statusCodes[402] = "Payment Required";
	this->_statusCodes[403] = "Forbidden";
	this->_statusCodes[404] = "Not Found";
	this->_statusCodes[405] = "Method Not Allowed";
	this->_statusCodes[406] = "Not Acceptable";
	this->_statusCodes[407] = "Proxy Authentication Required";
	this->_statusCodes[408] = "Request Timeout";
	this->_statusCodes[409] = "Conflict";
	this->_statusCodes[410] = "Gone";
	this->_statusCodes[411] = "Length Required";
	this->_statusCodes[412] = "Precondition Failed";
	this->_statusCodes[413] = "Request Entity Too Large";
	this->_statusCodes[414] = "Request-URI Too Large";
	this->_statusCodes[415] = "Unsupported Media Type";
	this->_statusCodes[416] = "Request range not satisfiable";
	this->_statusCodes[417] = "Expectation Failed";

	this->_statusCodes[5]	= "Server Error - The server failed to fulfill an apparently valid request";
	this->_statusCodes[500] = "Internal Server Error";
	this->_statusCodes[501] = "Not Implemented";
	this->_statusCodes[502] = "Bad Gateway";
	this->_statusCodes[503] = "Service Unavailable";
	this->_statusCodes[504] = "Gateway Time-out";
	this->_statusCodes[505] = "HTTP Version Not Supported";

}

void	Response::setResponse(std::string const& response) { this->_response = response; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void Response::buildResponse(Request const& request, ServerContext const& conf) {
	
	(void)request;
	(void)conf;

	std::stringstream res;

	int statusCode = 200;
	std::string statusMessage = "OK";
	std::string headerName = "Content-Type";
	std::string headerValue = "text/html";
	std::stringstream bodyContent;
	
	bodyContent	<< "<!DOCTYPE html>\n"
				<< "<html>\n"
				<< "<head>\n"
				<< "<title>Page Title</title>\n"
				<< "</head>\n"
				<< "<body>\n"
				<< "\n"
				<< "<h1>This is a Heading</h1>\n"
				<< "<p>This is a paragraph.</p>\n"
				<< "\n"
				<< "</body>\n"
				<< "</html>\n";

	// Build the response message here
	res << "HTTP/1.1 " << statusCode << " " << statusMessage << "\r\n";
	res << headerName << ": " << headerValue << "\r\n";
	res << "Content-Length: " << bodyContent.str().size() << "\r\n";
	res << "\r\n";
	res << bodyContent.str();		
		
	this->_response = res.str();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::OUTPUT OPERATOR OVERLOAD::

std::ostream& operator<<(std::ostream& o, Response const& rhs)
{
	o << rhs.getResponse();
	return (o);
}
	