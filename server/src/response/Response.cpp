/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 10:34:31 by seozcan           #+#    #+#             */
/*   Updated: 2023/09/28 12:43:44 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Response::Response() {

	this->setStatusCodes("../neoserv/status.codes");
	this->setMimeTypes("../neoserv/mime.types");	
}

Response::Response(Request const& request, ServerContext const& conf) : _request(request), _serverContext(conf) {

	this->setStatusCodes("../neoserv/status.codes");
	this->setMimeTypes("../neoserv/mime.types");
	this->buildResponse();
}

Response::Response(Response const& rhs) {

	if (this != &rhs)
		*this = rhs;
}

Response::~Response() {}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMPARISON OPERATORS::

Response& Response::operator=(Response const& rhs)
{
	if (this != &rhs)
	{
		this->_response = rhs.getResponse();
		this->_statusLine = rhs.getStatusLine();
		this->_statusCodes = rhs.getStatusCodes();
		this->_mimeTypes = rhs.getMimeTypes();
		this->_version = rhs.getVersion();
	}
	return (*this);
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

t_lexicon				Response::getStatusCodes() const { return (this->_statusCodes); }
t_lexicon				Response::getMimeTypes() const { return (this->_mimeTypes); }
std::string const & 	Response::getStatusCode(std::string const& code) const { return (this->_statusCodes.find(code)->second); }
std::string const &		Response::getMimeType(std::string const& extension) const { return (this->_mimeTypes.find(extension)->second); }
std::string const &		Response::getVersion() const { return (this->_version); }
std::string const &		Response::getResponse() const {	return (this->_response); }
std::string const &		Response::getStatusLine() const { return (this->_statusLine); }
ServerContext const &	Response::getServerContext() const { return (this->_serverContext); }
Request const &			Response::getRequest() const { return (this->_request); }


void	Response::setStatusLine(std::string const& statusLine) { this->_statusLine = statusLine; }
void	Response::setStatusCodes(std::string const& filename) { this->_statusCodes = this->_initFile(filename); }
void	Response::setMimeTypes(std::string const& filename) { this->_mimeTypes = this->_initFile(filename); }
void	Response::setResponse(std::string const& response) { this->_response = response; }
void	Response::setVersion(std::string const& version) { this->_version = version; }


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void Response::buildResponse() {
	

	std::stringstream res;

	int 				statusCode		= 200;
	std::string 		statusMessage	= "OK";
	std::string 		headerName		= "Content-Type";
	std::string 		headerValue		= "text/html";
	std::stringstream 	bodyContent;
	
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
	res << "HTTP/1.1 " 			<< statusCode				<< " " << statusMessage	<< "\r\n";
	res << headerName 			<< ": "						<< headerValue			<< "\r\n";
	res << "Content-Length: "	<< bodyContent.str().size()							<< "\r\n";
	res << "\r\n";
	res << bodyContent.str();		
		
	this->_response = res.str();
}

std::string Response::_trim(std::string const& str)
{
    const std::string ws = " \n\r\t\f\v";
    
    size_t start = str.find_first_not_of(ws);
    if (start == std::string::npos)
        return ""; // no content except whitespace

    size_t end = str.find_last_not_of(ws);
    
    return str.substr(start, end-start+1);
}

t_lexicon Response::_initFile(std::string const& filename) {

	std::ifstream	file(filename.c_str());

	if (!file.is_open())
		throw std::runtime_error("Could not open " +  filename + " file");

	t_lexicon		fileLexicon;
	std::string		line;
	
	while (std::getline(file, line) && !file.eof())
	{

		if (line.empty())
			continue;

		if (line.find_first_of('{' || '}') != std::string::npos)
			continue;
		
		std::string			key;
		std::string			value;
		std::stringstream 	sstream(line);
		
		key.clear();
		value.clear();

		sstream >> key;
		std::getline(sstream, value, ';');
		value = this->_trim(value);
		fileLexicon[key] = value;
	}
	
	file.close();
	std::cout << "file:\n" << print_map(fileLexicon) << std::endl;

	return (fileLexicon);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::OUTPUT OPERATOR OVERLOAD::

std::ostream& operator<<(std::ostream& o, Response const& rhs) {
	o << rhs.getResponse();
	return (o);
}
	