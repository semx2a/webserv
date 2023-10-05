#include "HttpStatus.hpp"

HttpStatus::HttpStatus() {

	this->_statusCode = "200";
}

HttpStatus::HttpStatus(HttpStatus const& src) {

	*this = src;
}

HttpStatus::HttpStatus(std::string const& statusCode) {

	this->_statusCode = statusCode;
}

HttpStatus::~HttpStatus() throw() {}

HttpStatus&		HttpStatus::operator=(HttpStatus const& rhs) {

	if (this != &rhs)
		this->_statusCode = rhs._statusCode;
	return *this;
}

std::string const&	HttpStatus::statusCode() { return _statusCode; }

void 				HttpStatus::setStatusCode(std::string const& statusCode) { _statusCode = statusCode; }

const char*			HttpStatus::what() const throw() { return this->_statusCode.c_str(); }

std::string HttpStatus::_statusCode = "200";