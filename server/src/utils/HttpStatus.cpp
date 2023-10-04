#include "HttpStatus.hpp"

HttpStatus::HttpStatus(std::string const& statusCode) {

	this->_statusCode = statusCode;
}

HttpStatus::~HttpStatus() throw() {}

std::string const&	HttpStatus::statusCode() { return _statusCode; }

const char*			HttpStatus::what() const throw() { return this->_statusCode.c_str(); }

std::string HttpStatus::_statusCode = "200";