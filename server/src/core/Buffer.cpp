#include "Buffer.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Buffer::Buffer() {

	_isTransferEncoding = false;
	_hasBody = false;
	_contentLength = 0;
}

Buffer::Buffer(Buffer const& rhs) {

	*this = rhs;
}


Buffer&	Buffer::operator=(Buffer const& rhs) {

	if (this != &rhs) {

		this->_raw = rhs.getRaw();
		this->_str = rhs.getStr();
		this->_hasBody = rhs.hasBody();
		this->_hasContentLength = rhs.hasContentLength();
		this->_contentLength = rhs.contentLength();
		this->_isTransferEncoding = rhs.isTransferEncoding();
		this->_isEnded = rhs.isRequestEnded();
	}
	return *this;
}


Buffer::~Buffer() {
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

std::string const&			Buffer::getStr() const { return _str; }
std::vector<char> const&	Buffer::getRaw() const { return _raw; }
bool						Buffer::hasBody() const { return _hasBody; }
bool						Buffer::hasContentLength() const { return _hasContentLength; }
size_t						Buffer::contentLength() const { return _contentLength; }
bool						Buffer::isTransferEncoding() const { return _isTransferEncoding; }
bool						Buffer::isRequestEnded() const { return _isEnded; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void		Buffer::setContentLength(size_t contentLength) { _contentLength = contentLength; }
void		Buffer::setRaw(std::vector<char> request) { _raw = request; }
void		Buffer::setStr(std::string requestStr) { _str = requestStr; }
void		Buffer::setHasBody(bool hasBody) { _hasBody = hasBody; }
void		Buffer::setHasContentLength(bool hasContentLength) { _hasContentLength = hasContentLength; }
void		Buffer::setIsTransferEncoding(bool isTransferEncoding) { _isTransferEncoding = isTransferEncoding; }
void		Buffer::setIsEnded(bool isEnded) { _isEnded = isEnded; }


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::OTHERS

void		Buffer::add(std::vector<char> raw) { 
	
	_raw.insert(_raw.end(), raw.begin(), raw.end()); 
}

void		Buffer::checkEnd() {

	if (this->_hasBody) {
		
		if (this->_isTransferEncoding) {
			this->_checkEndTransferEncoding();
		}
		else if (this->hasContentLength ()) {
			this->_checkEndContentLength();
		}
		return ;
	}

	std::string	_request_str (this->_raw.begin (), this->_raw.end());
	_str = _request_str;

	if (this->_raw.size() < 4)
		return;

	if (_str.find(DB_CRLF)) {

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
		else {
			_isEnded = true;
			std::cout << "Request is ended" << std::endl;
		}
	}
}

void	Buffer::_searchContentLength() {

	size_t content_length_pos = _str.find("Content-Length: ");
	if (content_length_pos != std::string::npos) {
		this->setContentLength (std::atoll(_str.substr(content_length_pos + 16).c_str()));
		std::cout << "Content-Length found: " << this->_contentLength << std::endl;
		this->_hasBody = true;
	}
}

void	Buffer::_searchTransferEncoding() {

	if (_str.find("Transfer-Encoding: ") != std::string::npos) {
		this->_isTransferEncoding = true;	
		std::cout << "Transfer Encoding found" << std::endl;
		this->_hasBody = true;
	}
}

void	Buffer::_checkEndContentLength() {

	if (this->_raw.size() >= this->_contentLength) {
		this->_isEnded = true;
	}
}

void	Buffer::_checkEndTransferEncoding() {

	if (_str.find("0\r\n\r\n"))
		_isEnded = true;
}
