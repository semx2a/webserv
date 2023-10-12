#include "AContext.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

AContext::AContext() : _autoindex("none"), _maxBodySize(1048576) {

		this->setPwd(getcwd(NULL, 0));
		this->setPwd(this->_pwd.replace(this->pwd().rfind("server"), this->_pwd.size() - std::string("server").size(), "www/"));
}

AContext::AContext(AContext const& rhs) { *this = rhs; }

AContext& AContext::operator=(AContext const& rhs) { 

	if (this != &rhs) {

		this->_autoindex = rhs.autoindex();
		this->_maxBodySize = rhs.maxBodySize();
		this->_pwd = rhs.pwd();
		this->_root = rhs.root();
		this->_errorPages = rhs.errorPages();
		this->_index = rhs.index();
		this->_authorizedMethods = rhs.authorizedMethods();
		this->_uploadFolder = rhs.uploadFolder();
	}
	return *this;
}

AContext::~AContext() {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

std::string const&								AContext::autoindex(void) const { return this->_autoindex; }
size_t											AContext::maxBodySize(void) const { return this->_maxBodySize; }
std::string const &								AContext::pwd(void) const { return this->_pwd; }
std::string const &								AContext::root(void) const { return this->_root; }
std::map<int, std::string> const &				AContext::errorPages(void) const { return this->_errorPages; }
std::vector<std::string> const & 				AContext::index(void) const { return this->_index; }
std::vector<std::string> const &				AContext::authorizedMethods(void) const { return this->_authorizedMethods; }
std::string const&								AContext::uploadFolder(void) const { return this->_uploadFolder; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	AContext::setAutoindex(std::string const& autoindex) { this->_autoindex = autoindex; }
void	AContext::setMaxBodySize(size_t size) { this->_maxBodySize = size; }
void	AContext::setPwd(std::string const& pwd) { this->_pwd = pwd; }
void	AContext::setRoot(std::string const& root) { this->_root = root; }
void	AContext::addErrorPage(int code, std::string const& path) { this->_errorPages[code] = path; }
void	AContext::setErrorPages(std::map<int, std::string> const& errorPages) { this->_errorPages = errorPages; }
void	AContext::addIndex(std::string const& index) { this->_index.push_back(index); }
void	AContext::setIndex(std::vector<std::string> const& index) { this->_index = index; }
void	AContext::addAuthorizedMethod(std::string const& method) { this->_authorizedMethods.push_back(method); }
void	AContext::setAuthorizedMethods(std::vector<std::string> const& authorizedMethods) { this->_authorizedMethods = authorizedMethods; }
void	AContext::setUploadFolder(std::string const& uploadFolder) { this->_uploadFolder = uploadFolder; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void	AContext::setDefaults() {

	if (this->pwd().empty()) {
		this->setPwd(getcwd(NULL, 0));
		this->setPwd(this->_pwd.replace(this->pwd().rfind("server"), this->_pwd.size() - std::string("server").size(), "www/"));
	}
	if (this->_root.empty())
		this->_root = this->pwd() + "html";
	if (this->_root.find_last_of("/") != this->_root.size() - 1)
		this->_root += "/";

	if (this->_authorizedMethods.empty()) {
		this->_authorizedMethods.push_back("GET");
		this->_authorizedMethods.push_back("POST");
		this->_authorizedMethods.push_back("DELETE");
	}

	if (this->_errorPages.empty())
		this->_errorPages[404] = this->pwd() + (std::string)"error_page.html";
	else {
		for (std::map<int, std::string>::iterator it = this->_errorPages.begin(); it != this->_errorPages.end(); it++)
			it->second = this->pwd() + it->second;
	}

	if (this->_uploadFolder.find(this->pwd()) != 0)
		this->_uploadFolder = this->pwd() + this->_uploadFolder;
}
