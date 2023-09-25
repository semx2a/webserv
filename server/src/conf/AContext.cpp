#include "AContext.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

AContext::AContext() : _autoindex(false), _cgi(false), _maxBodySize(1048576) {

//	this->_locations["/"] = "../www/html";
//	this->_errorPages[404] = "../www/html/error/404.html";
//	//NOTE: mettre valeurs par defaut APRES le parsing, SI le vecteur est vide (pareil pour les autres vecteurs)
//	this->_index.push_back("index.html");
//	this->_authorizedMethods.push_back("GET");
//	this->_authorizedMethods.push_back("POST");
//	this->_authorizedMethods.push_back("DELETE");
//	//If the directive is not present then either *:80 is used if nginx runs with the superuser privileges, or *:8000 otherwise.
//	this->_listen["*"] = 80;
//	this->_serverNames.push_back("localhost");
}

AContext::AContext(AContext const& rhs) { *this = rhs; }

AContext& AContext::operator=(AContext const& rhs) { 

	if (this != &rhs) {

		this->_autoindex = rhs.getAutoindex();
		this->_cgi = rhs.getCgi();
		this->_maxBodySize = rhs.getMaxBodySize();
		this->_errorPages = rhs.getErrorPages();
		this->_index = rhs.getIndex();
		this->_root = rhs.getRoot();
		this->_authorizedMethods = rhs.getAuthorizedMethods();
	}
	return *this;
}

AContext::~AContext() {}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS

bool											AContext::getAutoindex(void) const { return this->_autoindex; }
bool											AContext::getCgi(void) const { return this->_cgi; }
size_t											AContext::getMaxBodySize(void) const { return this->_maxBodySize; }
std::string const &								AContext::getRoot(void) const { return this->_root; }
std::map<int, std::string> const &				AContext::getErrorPages(void) const { return this->_errorPages; }
std::vector<std::string> const & 				AContext::getIndex(void) const { return this->_index; }
std::vector<std::string> const &				AContext::getAuthorizedMethods(void) const { return this->_authorizedMethods; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SETTERS

void	AContext::setAutoindex(bool autoindex) { this->_autoindex = autoindex; }
void	AContext::setCgi(bool cgi) { this->_cgi = cgi; }
void	AContext::setMaxBodySize(size_t size) { this->_maxBodySize = size; }
void	AContext::setRoot(std::string const& root) { this->_root = root; }
void	AContext::addErrorPage(int code, std::string const& path) { this->_errorPages[code] = path; }
void	AContext::setErrorPages(std::map<int, std::string> const& errorPages) { this->_errorPages = errorPages; }
void	AContext::setIndex(std::vector<std::string> const& index) { this->_index = index; }
void	AContext::setAuthorizedMethods(std::vector<std::string> const& authorizedMethods) { this->_authorizedMethods = authorizedMethods; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void	AContext::setDefaults() {
	
	if (this->_root.empty())
		this->_root = "../www/html/";
	if (this->_index.empty())
		this->_index.push_back("index.html");
	if (this->_authorizedMethods.empty()) {
		this->_authorizedMethods.push_back("GET");
		this->_authorizedMethods.push_back("POST");
		this->_authorizedMethods.push_back("DELETE");
	}
	if (this->_errorPages.empty())
		this->_errorPages[404] = "../www/html/404.html";
}
