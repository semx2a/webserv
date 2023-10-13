#include "Buffer.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Buffer::Buffer() :						_hasBody(false),
										_isEnded(false),
										_contentLength(0),
										_transferEncoding(false),
										_maxBodySize(0),
										_headerSize(0),
										_remainingContentLength(0) {}

/* Buffer::Buffer(size_t maxBodySize) :	_hasBody(false),
										_isEnded(false),
										_contentLength(0),
										_transferEncoding(false),
										_maxBodySize(maxBodySize),
										_headerSize(0),
										_remainingContentLength(0) {} */

Buffer::Buffer(Buffer const& rhs) {

	*this = rhs;
}


Buffer&	Buffer::operator=(Buffer const& rhs) {

	if (this != &rhs) {

		this->_raw = rhs.raw();
		this->_str = rhs.str();
	}
	return *this;
}


Buffer::~Buffer() {
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

std::string const&			Buffer::str() const { return _str; }
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
	if (!_transferEncoding && !_contentLength != 0 && boundary().empty()) {
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
		std::cout << "boundary not found" << std::endl;
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
	size_t				key_pos = _str.find(key);
	
	if (key_pos != std::string::npos) {

		_contentLength = std::atoll(_str.substr(key_pos + key.size()).c_str());
		if (_contentLength > _maxBodySize) {
			throw HttpStatus("413");
		}
		_headerSize = _str.size() - _str.find(DB_CRLF) + 4; // to remove it from the raw when checking in EndContentLength
		_remainingContentLength = _contentLength;
	}
}

void Buffer::_searchTransferEncoding() {

	if (_str.find("Transfer-Encoding: ") != std::string::npos) {

		#ifdef DEBUG_BUFFER
		std::cout << "[DEBUG] Transfer Encoding found" << std::endl;
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

	if (_str.find("0\r\n\r\n"))
		_isEnded = true;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: RESET

void Buffer::clear() {

	_raw.clear();
	_str.clear();
	_boundary.clear();
	resetFlagsAndValues();
}

void Buffer::resetFlagsAndValues() {

	_maxBodySize = 0;
	_headerSize = 0;	
	_contentLength = 0;
	_remainingContentLength = 0;
	_transferEncoding = false;
	_isEnded = false;
}