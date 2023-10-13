#include "Buffer.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Buffer::Buffer() :						_hasBody(false),
										_isEnded(false),
										_contentLength(0),
										_transferEncoding(false),
										_maxBodySize(0),
										_headerSize(0),
										_remainingContentLength(0) {}

Buffer::Buffer(Buffer const& rhs) {

	*this = rhs;
}


Buffer&	Buffer::operator=(Buffer const& rhs) {

	if (this != &rhs) {

		this->_raw = rhs.raw();
		this->_hasBody = rhs.hasBody();
		this->_isEnded = rhs.isEnded();
		this->_boundary = rhs.boundary();
		this->_contentLength = rhs.contentLength();
		this->_transferEncoding = rhs.transferEncoding();
		this->_maxBodySize = rhs.maxBodySize();
		this->_headerSize = rhs.headerSize();
		this->_remainingContentLength = rhs.remainingContentLength();
	}
	return *this;
}


Buffer::~Buffer() {
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

std::vector<char> const&	Buffer::raw() const { return _raw; }
bool						Buffer::hasBody() const { return _hasBody; }
bool						Buffer::isEnded() const { return _isEnded; }
std::string const&			Buffer::boundary() const { return _boundary; }
size_t						Buffer::contentLength() const { return _contentLength; }
bool						Buffer::transferEncoding() const { return _transferEncoding; }
size_t						Buffer::maxBodySize() const { return _maxBodySize; }
size_t						Buffer::headerSize() const { return _headerSize; }
t_ull						Buffer::remainingContentLength() const { return _remainingContentLength; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	Buffer::setMaxBodySize(size_t maxBodySize) { _maxBodySize = maxBodySize; }
void	Buffer::setRemainingContentLength(t_ull remainingContentLength) { _remainingContentLength = remainingContentLength; }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void		Buffer::add(std::vector<char> raw) { 

	_raw.insert(_raw.end(), raw.begin(), raw.end()); 
}

void Buffer::checkEnd() {

	if (_raw.size() < 4)
		return;
	if (_hasBody)
		processBodyEndCheck();
	else if (utl::searchVectorChar(_raw, "\r\n\r\n", 0) != -1) {
		searchForHeaders();
		processBodyEndCheck();
	}
}

void Buffer::searchForHeaders() {

	_searchBoundary();
	_searchTransferEncoding();
	_searchContentLength();
	if (!_transferEncoding and _contentLength == 0 and boundary().empty()) {
		_isEnded = true;
		return ;
	}
	_hasBody = true;
}

void Buffer::processBodyEndCheck() {

	if (not boundary().empty()) {
		_checkEndBoundary();
	}
 	else if (_transferEncoding) {
		_checkEndTransferEncoding();
	}
	else if (_contentLength != 0) {
		_checkEndContentLength();
	} 
}

void Buffer::_searchBoundary() {

	int boundary_position = utl::searchVectorCharUntil(_raw, 
														"boundary=", 
														utl::searchVectorChar(_raw, "\r\n\r\n", 0));
	if (boundary_position == -1) {
		return;
	}

	std::string		crlf = "\r\n";
    std::string     boundary(_raw.begin() + boundary_position + 9, 
								std::search(_raw.begin() + boundary_position, 
								_raw.end(), crlf.begin(), crlf.end()));

	#ifdef DEBUG_BUFFER
	std::cout << "[DEBUG] Boundary = " << boundary << std::endl;
	#endif

	boundary = "--" + boundary + "--";
	_boundary = boundary;

	#ifdef DEBUG_BUFFER
	std::cout << "[DEBUG] Ending boundary will be : " << boundary << std::endl;
	sleep(2);
	#endif
}

void Buffer::_searchContentLength() {

	const std::string	key = "Content-Length: ";
	size_t				key_pos = utl::searchVectorChar(_raw, key.c_str(), 0);
	std::string			str = utl::vectorOfCharToStr(_raw);
	
	if (key_pos != std::string::npos) {

		#ifdef DEBUG_BUFFER
		std::cout << "[DEBUG] Content Length found" << std::endl;
		sleep(2);
		#endif

		_contentLength = std::atoll(str.substr(key_pos + key.size()).c_str());
		if (_contentLength > _maxBodySize) {
			throw HttpStatus("413");
		}
		_headerSize = str.size() - str.find(DB_CRLF) + 4; // to remove it from the raw when checking in EndContentLength
		_remainingContentLength = _contentLength;
	}
}

void Buffer::_searchTransferEncoding() {

	if (utl::searchVectorChar(_raw, "Transfer-Encoding: chunked", 0) != -1) {

		#ifdef DEBUG_BUFFER
		std::cout << "[DEBUG] Transfer Encoding found" << std::endl;
		sleep(2);
		#endif

		_transferEncoding = true;	
	}
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CHECK END

void Buffer::_checkEndBoundary() {

	if (utl::searchVectorChar(_raw, _boundary.c_str(), 0) == -1) {

		#ifdef DEBUG_BUFFER
		std::cout << utl::vectorOfCharToStr(_raw) << std::endl;
		std::cout << RED << "[DEBUG] boundary not found" << std::endl;
		sleep(2);
		#endif

		return;
	}
	#ifdef DEBUG_BUFFER
	std::cout << GREEN << "[DEBUG] boundary found" << std::endl;
	#endif

	_isEnded = true;
}

void Buffer::_checkEndContentLength() {

	if (_raw.size() - _headerSize > _contentLength) {
		throw HttpStatus("413");
	}
	else if (_raw.size() - _headerSize == _contentLength) {
		_isEnded = true;
	}
}

void Buffer::_checkEndTransferEncoding() {

	if (utl::searchVectorChar(_raw, "0\r\n\r\n", 0) != -1)
		_isEnded = true;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: RESET

void Buffer::clear() {

	_raw.clear();
	_boundary.clear();
	resetFlagsAndValues();
}

void Buffer::resetFlagsAndValues() {

	_hasBody = false;
	_isEnded = false;
	_contentLength = 0;
	_transferEncoding = false;
	// we dont reset maxBodySize coz it is set in Engine::_addNewClient so will not be set again for the same client
	_headerSize = 0;
	_remainingContentLength = 0;
}