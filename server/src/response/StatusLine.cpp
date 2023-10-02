#include "StatusLine.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

StatusLine::StatusLine(std::string const& status_code) : ARespComponent(),
												_statusCode(status_code), 
												_reasonPhrase(StatusCodes::statusMessage(status_code)) {}

StatusLine::StatusLine(StatusLine const & rhs) : ARespComponent(rhs),
												_statusCode(rhs._statusCode), 
												_reasonPhrase(rhs._reasonPhrase) {
	*this = rhs;
}

StatusLine::~StatusLine() {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATORS::

StatusLine &		StatusLine::operator=(StatusLine const & rhs) {

	if (this != &rhs) {
	}
	return *this;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

void				StatusLine::setStatusCode(std::string const& statusCode) { _statusCode = statusCode; }
void				StatusLine::setReasonPhrase(std::string const & reasonPhrase) { _reasonPhrase = reasonPhrase; }

std::string const&		StatusLine::statusCode() const { return _statusCode; }
std::string	const&		StatusLine::reasonPhrase() const { return _reasonPhrase; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void				StatusLine::build() {

	std::stringstream statusLine;

	statusLine << "HTTP/1.1 ";
	statusLine << _statusCode << " ";
	statusLine << _reasonPhrase;
	statusLine << CRLF;

	this->setMessage(statusLine.str());
}