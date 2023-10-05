#include "Buffer.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

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

		this->_raw = rhs.raw();
		this->_str = rhs.str();
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


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

std::string const&			Buffer::str() const { return _str; }
std::vector<char> const&	Buffer::raw() const { return _raw; }
size_t						Buffer::maxBodySize() const { return _maxBodySize; }
bool						Buffer::hasBody() const { return _hasBody; }
bool						Buffer::hasContentLength() const { return _hasContentLength; }
t_ull						Buffer::remainingContentLength() const { return _contentLength - (_raw.size() - _headerSize); }
size_t						Buffer::contentLength() const { return _contentLength; }
size_t						Buffer::headerSize() const { return _headerSize; }
bool						Buffer::isTransferEncoding() const { return _isTransferEncoding; }
bool						Buffer::isRequestEnded() const { return _isEnded; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void		Buffer::setContentLength(size_t contentLength) { _contentLength = contentLength; }
void		Buffer::setRaw(std::vector<char> request) { _raw = request; }
void		Buffer::setMaxBodySize(size_t maxBodySize) { _maxBodySize = maxBodySize; }
void		Buffer::setStr(std::string requestStr) { _str = requestStr; }
void		Buffer::setHasBody(bool hasBody) { _hasBody = hasBody; }
void		Buffer::setHasContentLength(bool hasContentLength) { _hasContentLength = hasContentLength; }
void		Buffer::setRemainingContentLength(t_ull remainingContentLength) { _remainingContentLength = remainingContentLength; }
void		Buffer::setHeaderSize(size_t headerSize) { _headerSize = headerSize; }
void		Buffer::setIsTransferEncoding(bool isTransferEncoding) { _isTransferEncoding = isTransferEncoding; }
void		Buffer::setIsEnded(bool isEnded) { _isEnded = isEnded; }


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::OTHERS

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

	if (_str.find(DB_CRLF) != std::string::npos) {

		this->_searchTransferEncoding();
		this->_searchContentLength();
		if (this->_isTransferEncoding) {
			this->_checkEndTransferEncoding();
		}
		else if (this->_hasContentLength) {
			this->_hasBody = true;
			this->_checkEndContentLength();
		}
		else {
			_isEnded = true;
		}
	}
}

void	Buffer::_searchContentLength() {

	size_t content_length_pos = _str.find("Content-Length: ");
	
	if (content_length_pos != std::string::npos) {

		this->setContentLength (std::atoll(_str.substr(content_length_pos + 16).c_str()));
		this->setRemainingContentLength (this->_contentLength);
		if (this->_contentLength > this->_maxBodySize) {
			throw HttpStatus("413");
		}
		this->_hasBody = true;
		this->_headerSize = this->_raw.size();
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

	if (this->_raw.size() - this->_headerSize >= this->_contentLength) {
		this->_isEnded = true;
	}
}

void	Buffer::_checkEndTransferEncoding() {

	if (_str.find("0\r\n\r\n"))
		_isEnded = true;
}
