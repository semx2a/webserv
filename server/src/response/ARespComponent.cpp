#include "ARespComponent.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

ARespComponent::ARespComponent() : _message("") {}

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

void			ARespComponent::setMessage(std::string const & message) { _message = message; }

std::string		ARespComponent::getMessage() const { return _message; }