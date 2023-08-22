#include "ClientData.hpp"
#include <cstdlib>

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

ClientData::ClientData() {

	_isTransferEncoding = false;
	_hasBody = false;
	_contentLength = 0;
}

ClientData::ClientData(ClientData const& rhs) {

	*this = rhs;
}


ClientData&	ClientData::operator=(ClientData const& rhs) {

	if (this != &rhs) {
		// TODO
	}
	return *this;
}


ClientData::~ClientData() {
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

bool						ClientData::hasBody() const { return _hasBody; }

bool						ClientData::hasContentLength() const { return _hasContentLength; }

size_t						ClientData::contentLength() const { return _contentLength; }

bool						ClientData::isTransferEncoding() const { return _isTransferEncoding; }

std::vector<char> const&	ClientData::getRequest() const { return _request; }

bool						ClientData::isRequestEnded() const { return _isEnded; }


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void		ClientData::setContentLength(size_t contentLength) { _contentLength = contentLength; }


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::OTHERS

void		ClientData::addToBuffer(std::vector<char> raw) { _request.insert(_request.end(), raw.begin(), raw.end()); 

	if (this->_hasBody) {
		
		if (this->_isTransferEncoding) {
			this->_checkEndTransferEncoding();
		}
		else if (this->hasContentLength ()) {
			this->_checkEndContentLength();
		}
		return ;
	}

	std::string	_request_str (this->_request.begin (), this->_request.end());
	_requestStr = _request_str;

	if (this->_request.size() < 4)
		return;

	if (_requestStr.find(DB_CRLF)) {

		this->_searchTransferEncoding();
		this->_searchContentLength();
		if (this->_isTransferEncoding) {
			this->_checkEndTransferEncoding();
			return;
		}
		else if (this->_hasContentLength) {
			this->_hasBody = true;
			this->_checkEndContentLength();
			return;
		}
		else 
			_isEnded = true;
	}
}

void	ClientData::_searchContentLength() {

	size_t content_length_pos = _requestStr.find("Content-Length: ");
	if (content_length_pos != std::string::npos) {
		this->setContentLength (std::atoll(_requestStr.substr(content_length_pos + 16).c_str()));
		std::cout << "Content-Length found: " << this->_contentLength << std::endl;
		this->_hasBody = true;
	}
}

void	ClientData::_searchTransferEncoding() {

	if (_requestStr.find("Transfer-Encoding: ") != std::string::npos) {
		this->_isTransferEncoding = true;	
		std::cout << "Transfer Encoding found" << std::endl;
		this->_hasBody = true;
	}
}

void	ClientData::_checkEndContentLength() {

	if (this->_request.size() >= this->_contentLength) {
		this->_isEnded = true;
	}
}

void	ClientData::_checkEndTransferEncoding() {

	if (_requestStr.find("0\r\n\r\n"))
		_isEnded = true;
}
