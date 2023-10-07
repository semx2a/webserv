#include "Body.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Body::Body() : ARespComponent(), _contentLength(0) {}

Body::Body(std::string const & content) : ARespComponent(content), _contentLength(content.length()) {}

Body::Body(std::vector<char> const & content) : ARespComponent(content) {

	_contentLength = 0;
	std::vector<char>::const_iterator it = content.begin();
	while (it++ != content.end()) {
		_contentLength++;
	}
}

Body::Body(Body const & rhs) : ARespComponent(rhs) {
	
	*this = rhs;
}

Body::~Body() {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATORS::

Body &	Body::operator=(Body const & rhs) {

	if (this != &rhs) {
		ARespComponent::operator=(rhs);
		_contentLength = rhs.getContentLength();
	}
	return *this;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

size_t	Body::getContentLength() const { return _contentLength; }

void	Body::setContentLength(size_t contentLength) { _contentLength = contentLength; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void	Body::build(std::string const & content) {

	this->setContent(content);
	this->setContentLength(_content.length());
}

void	Body::build(std::vector<char> const & content) {

	this->setContent(content);

	_contentLength = 0;
	std::vector<char>::const_iterator it = content.begin();
	while (it++ != content.end()) {
		_contentLength++;
	}
}

void	Body::build() {}