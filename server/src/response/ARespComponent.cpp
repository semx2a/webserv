#include "ARespComponent.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

ARespComponent::ARespComponent() : _content("") {}

ARespComponent::ARespComponent(std::string const & content) : _content(content) {}

ARespComponent::ARespComponent(std::vector<char> const & content) : _content(std::string(content.begin(), content.end())) {

}

ARespComponent::ARespComponent(ARespComponent const & src) {
	
	*this = src;
}

ARespComponent::~ARespComponent() {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATORS::

ARespComponent &		ARespComponent::operator=(ARespComponent const & rhs) {

	if (this != &rhs) {
		_content = rhs.getContent();
	}
	return *this;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

void					ARespComponent::setContent(std::string const & content) { _content = content; }

void					ARespComponent::setContent(std::vector<char> const & content) { _content = std::string(content.begin(), content.end()); }

std::string const &		ARespComponent::getContent() const { return _content; }
