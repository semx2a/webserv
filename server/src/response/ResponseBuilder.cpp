#include "ResponseBuilder.hpp"

ResponseBuilder::ResponseBuilder() : _response(NULL) {}

ResponseBuilder::ResponseBuilder(Response* response) : _response (response) {}

ResponseBuilder::ResponseBuilder(ResponseBuilder const& rhs) {

	*this = rhs;
}

ResponseBuilder::~ResponseBuilder() {}

ResponseBuilder& ResponseBuilder::operator=(ResponseBuilder const& rhs) {

	if (this != &rhs) {
		this->_response = rhs._response;
	}
	return (*this);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void	ResponseBuilder::buildResponse() {

	std::stringstream	res;

	this->_buildStatusLine();
	this->_buildBody();
	this->_buildHeaders();

	res << this->_response->getStatusLine();
	res << this->_response->getHeaders();
	res << this->_response->getBody();
	
	this->_response->setResponseStr(res.str());
}

void	ResponseBuilder::_buildStatusLine() {
	
	std::stringstream line;

	line << this->_response->getRequest().getVersion() + " ";
//	line << this->_response->getStatusCode() + " ";
//	line << this->_response->getStatusMessage(this->_response->getStatusCode());
	line << CRLF;
	
	this->_response->setStatusLine(line.str());
}

void	ResponseBuilder::_buildHeaders() {

	std::stringstream	headers;
	
	headers << "Content-Type: " << "text/html" << CRLF;
	headers << "Content-Length: " << this->_response->getBody().size() << CRLF;
	headers << CRLF;

	this->_response->setHeaders(headers.str());
}

void	ResponseBuilder::_buildBody() {
	
	std::stringstream 	body;
	
	body	<< "<!DOCTYPE html>\n"
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

	this->_response->setBody(body.str());
}
