#include "ResponseContext.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

ResponseContext::ResponseContext(Request const& request, ServerContext const& serverContext) :
																				_request(request),
																				_serverContext(serverContext),
																				_maxBodySize(1048576) {
	this->_target = this->_request.target();
	this->_path = this->_target;

	t_locationIterator locationEndIterator = this->_serverContext.locations().end();

	std::string cgi[2] = {".py", ".php"};
	for (int i = 0; i < 2; i++) {

		if (_path.find(cgi[i]) != std::string::npos) {
			std::string ext = cgi[i];
			t_locationIterator cgiLocIt = this->_serverContext.locations().find(ext);
			if (cgiLocIt != locationEndIterator) {
				this->_isCgi = true;
				this->_location = cgiLocIt->second;
				_locationDirectives();
				break;
			}
		}
	}
	if (_path.find("/upload") != std::string::npos) {
		this->_isUpload = true;
	}
	if (not this->_isCgi) {

		size_t currentLength = 0;
		t_locationIterator it = this->_serverContext.locations().begin();
		for (; it != locationEndIterator; ++it) {
			if (this->_target.find(it->first) != std::string::npos) {
				currentLength = it->first.length();
				if (currentLength > this->_location.name().length()) {
					this->_location = it->second;
				std::cout << "[DEBUG] location name =" << this->_location.name() << std::endl;
				}
			}
		}
		if (currentLength > 0) {
			_locationDirectives();
		}
	}
	_serverDirectives();

	if (_path.find("//") != std::string::npos) {
		size_t pos = _path.find("//");
		_path.replace(pos, 2, "/");
	}
	#ifdef DEBUG_RESPONSECONTEXT
		std::cout << BORANGE << "\n[DEBUG] ResponseContext::ResponseContext()\n" << *this << RESET << std::endl;
	#endif

}

ResponseContext::ResponseContext(ResponseContext const& rhs) :
														_request(rhs.request()),
														_serverContext(rhs.serverContext()) {
	*this = rhs;
}

ResponseContext::~ResponseContext() {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATORS::

ResponseContext &		ResponseContext::operator=(ResponseContext const & rhs) {

	if (this != &rhs) {
		this->_target = rhs.target();
		this->_path = rhs.path();
		this->_location = rhs.location();
		this->_root = rhs.root();
		this->_alias = rhs.alias();
		this->_uploadFolder = rhs.uploadFolder();
		this->_index = rhs.index();
		this->_errorPages = rhs.errorPages();
		this->_autoindex = rhs.autoindex();
		this->_maxBodySize = rhs.maxBodySize();
		this->_authorizedMethods = rhs.authorizedMethods();
		this->_isCgi = rhs.isCgi();
		this->_isUpload = rhs.isUpload();
	}
	return *this;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

Request const&						ResponseContext::request() const { return this->_request; }
ServerContext const&				ResponseContext::serverContext() const { return this->_serverContext; }
LocationContext	const&				ResponseContext::location() const { return this->_location; }
std::string const&					ResponseContext::target() const { return this->_target; }
std::string const&					ResponseContext::path() const { return this->_path; }
std::string const&					ResponseContext::root() const { return this->_root; }
bool								ResponseContext::alias() const { return this->_alias; }
std::string const&					ResponseContext::uploadFolder() const { return this->_uploadFolder; }
std::vector<std::string> const&		ResponseContext::index() const { return this->_index; }
std::map<int, std::string> const&	ResponseContext::errorPages() const { return this->_errorPages; }
std::string const&					ResponseContext::autoindex() const { return this->_autoindex; }
size_t 								ResponseContext::maxBodySize() const { return this->_maxBodySize; }
std::vector<std::string> const&		ResponseContext::authorizedMethods() const { return this->_authorizedMethods; }
bool								ResponseContext::isCgi() const { return this->_isCgi; }
bool								ResponseContext::isUpload() const { return this->_isUpload; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void	ResponseContext::_locationDirectives() {

	if (this->_location.alias() != "") {

		std::string tmp = this->_location.alias();
		size_t pos = this->_path.find(this->_location.name());
		this->_path.replace(pos, this->_location.name().size(), tmp);
		this->_alias = true;
	}

	else if (this->_location.root() != "") {
		this->_path = this->_location.root() + this->_path;
	}

	this->_root = this->_location.root();
	this->_index = this->_location.index();
	std::cout << "[DEBUG] upload folder: " << this->_location.uploadFolder() << std::endl;
	this->_uploadFolder = this->_location.uploadFolder();
	this->_errorPages = this->_location.errorPages();
	this->_autoindex = this->_location.autoindex();
	this->_maxBodySize = this->_location.maxBodySize();
	this->_authorizedMethods = this->_location.authorizedMethods();

}

void	ResponseContext::_serverDirectives() {

	// if empty = location not found OR some directives empty in location
	if (this->_root.empty()) {
		this->_root = this->_serverContext.root();
	}
	if (this->_path == this->_target) {
		this->_path = this->_serverContext.root() + this->_path;
	}
	if (this->_index.empty()) {
		this->_index = this->_serverContext.index();
	}
	if (this->_uploadFolder.empty()) {
		this->_uploadFolder = this->_serverContext.uploadFolder();
	}
	if (this->_errorPages.empty()) {
		this->_errorPages = this->_serverContext.errorPages();
	}
	if (this->_autoindex == "none")	{
		this->_autoindex = this->_serverContext.autoindex();
	}
	if (this->_maxBodySize == 1048576) {
		this->_maxBodySize = this->_serverContext.maxBodySize();
	}
	if (this->_authorizedMethods.empty()) {
		this->_authorizedMethods = this->_serverContext.authorizedMethods();
	}
}


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OUTPUT ::

std::ostream&	operator<<(std::ostream& o, ResponseContext const& rhs) {

	o << "\t" << ORANGE << "Method: " << PURPLE << rhs.request().method() << std::endl;
	o << "\t" << ORANGE << "Target: " << PURPLE << rhs.target() << std::endl; 
	o << "\t" << ORANGE << "Contains some specific directives due to location ? " << PURPLE;
		if (rhs.location().name().empty())
			o << "no -> server's directives only" << std::endl;
		else 
			o << "yes: location '" << rhs.location().name() << "'" << std::endl;
	o << "\t" << ORANGE << "Root: " << PURPLE << rhs.root() << std::endl;
	o << "\t=> " << ORANGE << "Path: " << PURPLE << rhs.path() << std::endl;
	o << "\t" << ORANGE << "Alias: " << PURPLE << rhs.alias() << std::endl;
	o << "\t" << ORANGE << "Upload folder: " << PURPLE << rhs.uploadFolder() << std::endl;
	o << "\t" << ORANGE << "Index: " << PURPLE << utl::print_vector(rhs.index()) << std::endl;
	o << "\t" << ORANGE << "Autoindex: " << PURPLE << rhs.autoindex() << std::endl;
	o << "\t" << ORANGE << "Error pages: " << PURPLE << utl::print_map(rhs.errorPages()) << std::endl;
	o << "\t" << ORANGE << "Max body size: " << PURPLE << rhs.maxBodySize() << std::endl;
	o << "\t" << ORANGE << "Authorized methods: " << PURPLE << utl::print_vector(rhs.authorizedMethods()) << std::endl;
	o << "\t" << ORANGE << "isCgi: " << PURPLE << rhs.isCgi() << std::endl;
	o << "\t" << ORANGE << "isUpload: " << PURPLE << rhs.isUpload() << std::endl;
	o << "\t" << RESET << std::endl;
	return o;
}