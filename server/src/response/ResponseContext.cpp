#include "ResponseContext.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

ResponseContext::ResponseContext(Request const& request, ServerContext const& serverContext) :
																				_request(request),
																				_serverContext(serverContext),
																				_maxBodySize(1048576) {
	this->_target = this->_request.target();
	this->_path = this->_target;

	std::string cgi[2] = {".py", ".php"};

	for (int i = 0; i < 2; i++) {

		if (_path.find(cgi[i]) != std::string::npos) {

			std::string ext = cgi[i].substr(1, cgi[i].size() - 1);
			t_locationIterator cgiLocIt = this->_serverContext.locations().find(ext);
			t_locationIterator endIt = this->_serverContext.locations().end();
			if (cgiLocIt != endIt) {
				std::cout << "[DEBUG] Is Cgi : " << cgi[i] << std::endl;
				this->_cgi = cgi[i];
				this->_location = cgiLocIt;
				_locationDirectives();
				break;
			}
		}
	}
	if (this->_cgi.empty()) {

		t_locationIterator it = this->_serverContext.locations().begin();
		t_locationIterator ite = this->_serverContext.locations().end();
		for (; it != ite; ++it) {
			if (this->_target.find(it->first) != std::string::npos) {
				this->_location = it;
				break;
			}
		}
		if (it != ite) {
			_locationDirectives();
		}
	}
	_serverDirectives();

	if (_path.find("//") != std::string::npos) {
		size_t pos = _path.find("//");
		_path.replace(pos, 2, "/");
	}
	#ifdef DEBUG_RESPONSECONTEXT
		std::cout << BORANGE << "[DEBUG] ResponseContext::ResponseContext()\n" << *this << RESET << std::endl;
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
		this->_root = rhs.root();
		this->_alias = rhs.alias();
		this->_index = rhs.index();
		this->_errorPages = rhs.errorPages();
		this->_autoindex = rhs.autoindex();
		this->_maxBodySize = rhs.maxBodySize();
		this->_authorizedMethods = rhs.authorizedMethods();
	}
	return *this;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

Request const&						ResponseContext::request() const { return this->_request; }
ServerContext const&				ResponseContext::serverContext() const { return this->_serverContext; }
t_locationIterator					ResponseContext::location() const { return this->_location; }
std::string const&					ResponseContext::target() const { return this->_target; }
std::string const&					ResponseContext::path() const { return this->_path; }
std::string const&					ResponseContext::root() const { return this->_root; }
bool								ResponseContext::alias() const { return this->_alias; }
std::vector<std::string> const&		ResponseContext::index() const { return this->_index; }
std::map<int, std::string> const&	ResponseContext::errorPages() const { return this->_errorPages; }
std::string const&					ResponseContext::autoindex() const { return this->_autoindex; }
size_t 								ResponseContext::maxBodySize() const { return this->_maxBodySize; }
std::vector<std::string> const&		ResponseContext::authorizedMethods() const { return this->_authorizedMethods; }
std::string const&					ResponseContext::cgi() const { return this->_cgi; }

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

void	ResponseContext::_locationDirectives() {

	if (this->_location->second.alias() != "") {

		std::string tmp = this->_location->second.alias();
		size_t pos = this->_path.find(this->_location->first);
		this->_path.replace(pos, this->_location->first.size(), tmp);
		this->_alias = true;
	}

	else if (this->_location->second.root() != "") {
		this->_path = this->_location->second.root() + this->_path;
	}

	this->_root = this->_location->second.root();
	this->_index = this->_location->second.index();
	this->_errorPages = this->_location->second.errorPages();
	this->_autoindex = this->_location->second.autoindex();
	this->_maxBodySize = this->_location->second.maxBodySize();
	this->_authorizedMethods = this->_location->second.authorizedMethods();

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
	o << "\t" << ORANGE << "Root: " << PURPLE << rhs.root() << std::endl;
	o << "\t" << ORANGE << "Alias: " << PURPLE << rhs.alias() << std::endl;
	o << "\t" << ORANGE << "Path: " << PURPLE << rhs.path() << std::endl;
	o << "\t" << ORANGE << "Index: " << PURPLE << utl::print_vector(rhs.index()) << std::endl;
	o << "\t" << ORANGE << "Autoindex: " << PURPLE << rhs.autoindex() << std::endl;
	o << "\t" << ORANGE << "Error pages: " << PURPLE << utl::print_map(rhs.errorPages()) << std::endl;
	o << "\t" << ORANGE << "Max body size: " << PURPLE << rhs.maxBodySize() << std::endl;
	o << "\t" << ORANGE << "Authorized methods: " << PURPLE << utl::print_vector(rhs.authorizedMethods()) << std::endl;
	o << "\t" << RESET << std::endl;
	return o;
}