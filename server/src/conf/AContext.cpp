#include "AContext.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

AContext::AContext() : _autoindex(false), _maxBodySize(1048576) {}

AContext::AContext(AContext const& rhs) { *this = rhs; }

AContext& AContext::operator=(AContext const& rhs) { 

	if (this != &rhs) {

		this->_autoindex = rhs.autoindex();
		this->_maxBodySize = rhs.maxBodySize();
		this->_errorPages = rhs.errorPages();
		this->_index = rhs.index();
		this->_root = rhs.root();
		this->_authorizedMethods = rhs.authorizedMethods();
	}
	return *this;
}

AContext::~AContext() {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

bool											AContext::autoindex(void) const { return this->_autoindex; }
size_t											AContext::maxBodySize(void) const { return this->_maxBodySize; }
std::string const &								AContext::root(void) const { return this->_root; }
std::map<int, std::string> const &				AContext::errorPages(void) const { return this->_errorPages; }
std::vector<std::string> const & 				AContext::index(void) const { return this->_index; }
std::vector<std::string> const &				AContext::authorizedMethods(void) const { return this->_authorizedMethods; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	AContext::setAutoindex(bool autoindex) { this->_autoindex = autoindex; }
void	AContext::setMaxBodySize(size_t size) { this->_maxBodySize = size; }
void	AContext::setRoot(std::string const& root) { this->_root = root; }
void	AContext::addErrorPage(int code, std::string const& path) { this->_errorPages[code] = path; }
void	AContext::setErrorPages(std::map<int, std::string> const& errorPages) { this->_errorPages = errorPages; }
void	AContext::addIndex(std::string const& index) { this->_index.push_back(index); }
void	AContext::setIndex(std::vector<std::string> const& index) { this->_index = index; }
void	AContext::addAuthorizedMethod(std::string const& method) { this->_authorizedMethods.push_back(method); }
void	AContext::setAuthorizedMethods(std::vector<std::string> const& authorizedMethods) { this->_authorizedMethods = authorizedMethods; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void	AContext::setDefaults() {
	
	if (this->_root.empty())
		this->_root = "../www";
	else 
		this->_root = "../www/" + this->_root;
	if (this->_authorizedMethods.empty()) {
		this->_authorizedMethods.push_back("GET");
		this->_authorizedMethods.push_back("POST");
		this->_authorizedMethods.push_back("DELETE");
		this->_authorizedMethods.push_back("ERROR");
	}
	if (this->_errorPages.empty())
		this->_errorPages[404] = "../www/html/404.html";
}
