#include "Headers.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Headers::Headers() : ARespComponent() {}

Headers::Headers(std::string const& contentType, size_t const & contentLength, ServerContext const& sc) {

	this->build(contentType, contentLength, sc);
}

Headers::Headers(Headers const & rhs) : ARespComponent(rhs) {
	
	*this = rhs;
}

Headers::~Headers() {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATORS::

Headers &		Headers::operator=(Headers const & rhs) {

	if (this != &rhs) {
		_message = rhs._message;
	}
	return *this;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void			Headers::build(std::string const& contentType, size_t const & contentLength, ServerContext const& sc) {

	(void)sc;
	std::stringstream headers;

	headers << "Content-Type: " << contentType << CRLF;
	headers << "Content-Length: " << contentLength << CRLF;
	headers << CRLF;

	this->setMessage(headers.str());
}

/* Sets default headers if constructor called with size_t contentLength */
void	Headers::build() {

	std::stringstream headers;

	headers << "Content-Type: text/html" << CRLF;
	headers << "Content-Length: " << this->getContentLength() << CRLF;
	headers << CRLF;

	this->setMessage(headers.str());
}