#include "StatusLine.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

StatusLine::StatusLine() : ARespComponent(), _statusCode(""), _reasonPhrase("") {}

StatusLine::StatusLine(std::string const& status_code, std::string const& reasonPhrase) : ARespComponent(), _statusCode(status_code), _reasonPhrase(reasonPhrase) {

	this->build();
}

StatusLine::StatusLine(StatusLine const & rhs) : ARespComponent(rhs), _statusCode(rhs.statusCode()), _reasonPhrase(rhs.reasonPhrase()) {
	
	*this = rhs;
}

StatusLine::~StatusLine() {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATORS::

StatusLine &		StatusLine::operator=(StatusLine const & rhs) {

	if (this != &rhs) {
		ARespComponent::operator=(rhs);
		_statusCode = rhs.statusCode();
		_reasonPhrase = rhs.reasonPhrase();
	}
	return *this;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

void				StatusLine::setStatusCode(std::string const& statusCode) { _statusCode = statusCode; }
void				StatusLine::setReasonPhrase(std::string const & reasonPhrase) { _reasonPhrase = reasonPhrase; }

std::string const&		StatusLine::statusCode() const { return _statusCode; }
std::string	const&		StatusLine::reasonPhrase() const { return _reasonPhrase; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void				StatusLine::build(std::string const& statusCode, std::string const& reasonPhrase) {

	this->setStatusCode(statusCode);
	this->setReasonPhrase(reasonPhrase);
	this->build();
}

void				StatusLine::build() {

	std::stringstream statusLine;

	statusLine << "HTTP/1.1 ";
	statusLine << _statusCode << " ";
	statusLine << _reasonPhrase;
	statusLine << CRLF;

	this->setContent(statusLine.str());
}