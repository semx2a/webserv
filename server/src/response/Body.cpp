#include "Body.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Body::Body() : ARespComponent() {}

Body::Body(std::string const & message) : ARespComponent(message) {}

Body::Body(std::vector<char> const & message) : ARespComponent(message) {}

Body::Body(Body const & rhs) : ARespComponent(rhs) {
	
	*this = rhs;
}

Body::~Body() {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATORS::

Body &	Body::operator=(Body const & rhs) {

	if (this != &rhs) {
		_message = rhs._message;
	}
	return *this;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void	Body::build(std::string const & message) {

	this->setMessage(message);
	this->setContentLength(message.length());

}

void	Body::build(std::vector<char> const & message) {

	this->setMessage(message);
	this->setContentLength(message.size());
}

void	Body::build() {

	this->setContentLength(this->getMessage().length());
}