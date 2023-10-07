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
		ARespComponent::operator=(rhs);
	}
	return *this;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

size_t				Headers::contentLength() const { return _contentLength; }

void				Headers::setContentLength(size_t contentLength) { _contentLength = contentLength; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void			Headers::build(std::string const& contentType, size_t const & contentLength, ServerContext const& sc) {

	(void)sc;
	std::stringstream headers;

	headers << "Content-Type: " << contentType << CRLF;
	headers << "Content-Length: " << contentLength << CRLF;
	headers << CRLF;

	this->setContent(headers.str());
}

/* Sets default headers if constructor called with size_t contentLength */
void	Headers::build() {

	std::stringstream headers;

	headers << "Content-Type: text/html" << CRLF;
	headers << "Content-Length: " << this->contentLength() << CRLF;
	headers << CRLF;

	this->setContent(headers.str());
}