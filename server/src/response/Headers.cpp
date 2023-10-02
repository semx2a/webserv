#include "Headers.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Headers::Headers() : ARespComponent() {}

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

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

void			Headers::setContentLength(size_t const & contentLength) { _contentLength = contentLength; }

size_t			Headers::getContentLength() const { return _contentLength; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void			Headers::build() {

	std::stringstream headers;

	headers << "Content-Type: text/html" << CRLF;
	headers << "Content-Length: " << _message.length() << CRLF;
	headers << CRLF;

	this->setMessage(headers.str());
}