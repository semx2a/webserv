#include "Headers.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Headers::Headers() : ARespComponent() {}

Headers::Headers(size_t contentLength) : ARespComponent() {

	this->setContentLength(contentLength);
	this->build();
}

Headers::Headers(std::string const& path, size_t contentLength, std::string const& contentType, ResponseContext const& rc, std::string const& filePath) : ARespComponent() {

	this->build(path, contentLength, contentType, rc, filePath);
}

Headers::Headers(Headers const & rhs) : ARespComponent(rhs) {
	
	*this = rhs;
}

Headers::~Headers() {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATORS::

Headers &		Headers::operator=(Headers const & rhs) {

	if (this != &rhs) {
		ARespComponent::operator=(rhs);
		
	}
	return *this;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

size_t			Headers::contentLength() const { return this->_contentLength; }

void			Headers::setContentLength(size_t contentLength) { this->_contentLength = contentLength; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

std::string	Headers::_findExtension(std::string const& path) {

	std::string ext = path.substr(path.rfind('.') + 1, path.size() - path.rfind('.') - 1);
	return (ext);
}

void	Headers::build(std::string const& path, size_t contentLength, std::string const& customHeaders, ResponseContext const& rc, std::string const& filePath) {

	std::stringstream headers;

	headers << "Date: " << utl::getDate() << CRLF;
	headers << "Server: " << rc.serverContext().serverNames()[0] << CRLF;
	if (contentLength != 0) {
		if (customHeaders.find("Content-Type") == std::string::npos)
			headers << "Content-Type: " << this->_mimeTypes.getMimeType(this->_findExtension(path)) << CRLF;
		headers << "Content-Length: " << contentLength << CRLF;
	}
	headers << "Connection: keep-alive" << CRLF;
	headers << "Keep-Alive: timeout=5, max=1000" << CRLF;
	if (customHeaders.find("Cache-Control") == std::string::npos)
		headers << "Cache-Control: no-cache" << CRLF;
	if (not filePath.empty())
		headers << "Location: " << filePath << CRLF;
	else
		headers << "Location: /" << CRLF;
	if (!customHeaders.empty())
		headers << customHeaders;
	headers << CRLF;

	this->setContent(headers.str());
}

void	Headers::build() {
	
	std::stringstream headers;

	headers << "Date: " << utl::getDate() << CRLF;
	headers << "Server: webserv" << CRLF;
	headers << "Content-Type: " << "text/html; charset=utf-8" << CRLF;
	headers << "Content-Length: " << this->contentLength() << CRLF;
	headers << "Connection: keep-alive" << CRLF;
	headers << "Keep-Alive: timeout=5, max=1000" << CRLF;
	headers << "Cache-Control: no-cache" << CRLF;
	headers << "Location: /" << CRLF;
	headers << CRLF;

	this->setContent(headers.str());
}