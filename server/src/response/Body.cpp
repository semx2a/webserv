#include "Body.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Body::Body() : ARespComponent() {}

Body::Body(Body const & rhs) : ARespComponent(rhs) {
	
	*this = rhs;
}

Body::~Body() {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATORS::

Body &		Body::operator=(Body const & rhs) {

	if (this != &rhs) {
		_message = rhs._message;
	}
	return *this;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void		Body::build() {
	
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

	this->setMessage(body.str());
}