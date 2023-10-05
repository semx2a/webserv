#include "ARespComponent.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

ARespComponent::ARespComponent() : _message("") {}

ARespComponent::ARespComponent(std::string const & message) : _message(message) {}

ARespComponent::ARespComponent(std::vector<char> const & message) : _message(std::string(message.begin(), message.end())) {}

ARespComponent::ARespComponent(ARespComponent const & src) {
	
	*this = src;
}

ARespComponent::~ARespComponent() {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATORS::

ARespComponent &		ARespComponent::operator=(ARespComponent const & rhs) {

	if (this != &rhs) {
		_message = rhs._message;
	}
	return *this;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

void					ARespComponent::setMessage(std::string const & message) { _message = message; }

void					ARespComponent::setMessage(std::vector<char> const & message) { _message = std::string(message.begin(), message.end()); }

void					ARespComponent::setContentLength(size_t const & contentLength) { _contentLength = contentLength; }

std::string const &		ARespComponent::getMessage() const { return _message; }

size_t					ARespComponent::getContentLength() const { return _contentLength; }
