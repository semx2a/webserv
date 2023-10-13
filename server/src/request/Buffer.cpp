#include "Buffer.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Buffer::Buffer() : 	_maxBodySize(0),
					_contentLength(0),
					_remainingContentLength(0),
					_headerSize(0),
					_isTransferEncoding(false),
					_isEnded(false),
					_isBoundary(false),
					_hasBody(false),
					_hasContentLength(false)
					{}

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


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void		Buffer::add(std::vector<char> raw) { 
	_raw.insert(_raw.end(), raw.begin(), raw.end()); 
}

void Buffer::checkEnd() {
	if (_raw.size() < 4)
		return;
	if (_hasBody) {
		processBodyEndCheck();
		return;
	}
	_str.assign(_raw.begin(), _raw.end());
	if (_str.find(DB_CRLF) != std::string::npos) {
		searchForHeaders();
		processBodyEndCheck();
	}
}

void Buffer::searchForHeaders() {
	_searchBoundary();
	_searchTransferEncoding();
	_searchContentLength();
	if (!_isTransferEncoding && !_hasContentLength && !_isBoundary) {
		_isEnded = true;
	}
}

void Buffer::processBodyEndCheck() {
	if (_isBoundary) {
		_checkEndBoundary();
	}
 	else if (_isTransferEncoding) {
		_checkEndTransferEncoding();
	}
	else if (_hasContentLength) {
		_checkEndContentLength();
	} 
}
void Buffer::_searchBoundary() {
	int boundary_position = utl::searchVectorCharUntil(_raw, 
														"boundary=", 
														utl::searchVectorChar(_raw, "\r\n\r\n", 0));
	if (boundary_position == -1) {
		std::cout << "boundary not found" << std::endl;
		return;
	}

	std::string		crlf = "\r\n";
    std::string     boundary(_raw.begin() + boundary_position + 9, 
								std::search(_raw.begin() + boundary_position, 
								_raw.end(), crlf.begin(), crlf.end()));

	#ifdef DEBUG_BUFFER
	std::cout << "boundary = " << boundary << std::endl;
	#endif

	boundary = "--" + boundary + "--";
	_isBoundary = true;
	_hasBody = true;
	_boundary = boundary;

	#ifdef DEBUG_BUFFER
	std::cout << "ending boundary will be : " << boundary << std::endl;
	sleep(2);
	#endif
}

void Buffer::_searchContentLength() {
	const std::string CONTENT_LENGTH = "Content-Length: ";
	size_t content_length_pos = _str.find(CONTENT_LENGTH);
	
	if (content_length_pos != std::string::npos) {
		setContentLength(std::atoll(_str.substr(content_length_pos + CONTENT_LENGTH.size()).c_str()));
		setRemainingContentLength(_contentLength);
		if (_contentLength > _maxBodySize) {
			throw HttpStatus("413");
		}
		_hasContentLength = true;
		_hasBody = true;
		_headerSize = _str.size() - _str.find(DB_CRLF) + 4;
	}
}

void Buffer::_searchTransferEncoding() {

	if (_str.find("Transfer-Encoding: ") != std::string::npos) {

		_isTransferEncoding = true;	
		_hasBody = true;

		#ifdef DEBUG_BUFFER
		std::cout << "Transfer Encoding found" << std::endl;
		#endif
	}
}


void Buffer::_checkEndBoundary() {

	if (utl::searchVectorChar(_raw, _boundary.c_str(), 0) == -1) {

		#ifdef DEBUG_BUFFER
		std::cout << utl::vectorOfCharToStr(_raw) << std::endl;
		std::cout << RED << "boundary not found" << std::endl;
		sleep(2);
		#endif

		return;
	}
	#ifdef DEBUG_BUFFER
	std::cout << GREEN << "boundary found" << std::endl;
	#endif

	_isEnded = true;
}

void Buffer::_checkEndContentLength() {
	#ifdef DEBUG_BUFFER
	std::cout << BORANGE << "checkEndContentLength" << std::endl;
	std::cout << ORANGE << "this->raw().size() = " << raw().size() << std::endl;
	std::cout << "this->_headerSize = " << _headerSize << std::endl;
	std::cout << "this->_contentLength = " << _contentLength << std::endl;
	std::cout << RESET << std::endl;
	#endif

	if (_raw.size() - _headerSize >= _contentLength) {
		_isEnded = true;
	}
}

void Buffer::_checkEndTransferEncoding() {
	if (_str.find("0\r\n\r\n"))
		_isEnded = true;
}

void Buffer::clear() {
	_raw.clear();
	_str.clear();
	resetFlagsAndValues();
}

void Buffer::resetFlagsAndValues() {
	_hasBody = false;
	_hasContentLength = false;
	_contentLength = 0;
	_remainingContentLength = 0;
	_headerSize = 0;
	_isTransferEncoding = false;
	_isEnded = false;
}


/* void		Buffer::checkEnd() {

	if (this->_hasBody) {
		
		if (this->_isTransferEncoding) {
			this->_checkEndTransferEncoding();
		}
		else if (this->hasContentLength ()) {
			this->_checkEndContentLength();
		}
		else if (this->_isBoundary) {
			this->_checkEndBoundary();
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
		this->_searchBoundary();
		if (this->_isTransferEncoding) {
			this->_checkEndTransferEncoding();
		}
		else if (this->_hasContentLength) {
			this->_hasBody = true;
			this->_checkEndContentLength();
		}
		else if (this->_isBoundary) {
			this->_hasBody = true;
			this->_checkEndBoundary();
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
		this->_hasContentLength = true;
		this->_hasBody = true;
		this->_headerSize = _str.size() - _str.find(DB_CRLF) + 4;
	}
}

void	Buffer::_searchTransferEncoding() {

	if (_str.find("Transfer-Encoding: ") != std::string::npos) {
		this->_isTransferEncoding = true;	
		std::cout << "Transfer Encoding found" << std::endl;
		this->_hasBody = true;
	}
}

void	Buffer::_searchBoundary() {

    int	boundary_position = utl::searchVectorCharUntil(_raw, "boundary=", utl::searchVectorChar(_raw, "\r\n\r\n", 0));
    if (boundary_position == -1)
		std::cout << "boundary not found" << std::endl;

	std::string		crlf = "\r\n";
    std::string     boundary(_raw.begin() + boundary_position + 9, std::search(_raw.begin() + boundary_position, _raw.end(), crlf.begin(), crlf.end()));
    
    boundary = "--" + boundary + "--";
	std::cout << "boundary = " << boundary << std::endl;
	_isBoundary = true;
	_boundary = boundary;
}

void	Buffer::_checkEndBoundary() {

	if (utl::searchVectorChar(_raw, _boundary.c_str(), 0) == -1) {
		utl::vectorOfCharToStr(_raw);
		std::cout << "boundary not found" << std::endl;
		return ;
	}
	std::cout << "boundary found" << std::endl;
	_isEnded = true;

}

void	Buffer::_checkEndContentLength() {

	#ifdef DEBUG_BUFFER
	std::cout << BORANGE << "checkEndContentLength" << std::endl;
	std::cout << ORANGE << "this->raw().size() = " << this->raw().size() << std::endl;
	std::cout << "this->_headerSize = " << this->_headerSize << std::endl;
	std::cout << "this->_contentLength = " << this->_contentLength << std::endl;
	std::cout << RESET << std::endl;
	#endif
	if (this->_raw.size() - this->_headerSize >= this->_contentLength) {
		this->_isEnded = true;
	}
}

void	Buffer::_checkEndTransferEncoding() {

	if (_str.find("0\r\n\r\n"))
		_isEnded = true;
}

void	Buffer::clear() {
	
	_raw.clear();
	_str.clear();
	_hasBody = false;
	_hasContentLength = false;
	_contentLength = 0;
	_remainingContentLength = 0;
	_headerSize = 0;
	_isTransferEncoding = false;
	_isEnded = false;
} */