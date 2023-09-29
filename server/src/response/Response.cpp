#include "Response.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Response::Response() : _statusCode("200"), _version("HTTP/1.1") {

	this->setStatusMessages("../neoserv/status.codes");
	this->setMimeTypes("../neoserv/mime.types");	
}

Response::Response(Request const& request, ServerContext const& conf) : _request(request), _serverContext(conf), _statusCode("200"), _version("HTTP/1.1") {

	this->setStatusMessages("../neoserv/status.codes");
	this->setMimeTypes("../neoserv/mime.types");
}

Response::Response(Response const& rhs) {

	if (this != &rhs)
		*this = rhs;
}

Response::~Response() {}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMPARISON OPERATORS::

Response& Response::operator=(Response const& rhs)
{
	if (this != &rhs)
	{
		this->_response = rhs.getResponse();
		this->_statusLine = rhs.getStatusLine();
		this->_statusMessages = rhs.getStatusMessages();
		this->_mimeTypes = rhs.getMimeTypes();
	}
	return (*this);
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

Request const &			Response::getRequest() const { return (this->_request); }
ServerContext const &	Response::getServerContext() const { return (this->_serverContext); }
t_lexicon				Response::getStatusMessages() const { return (this->_statusMessages); }
t_lexicon				Response::getMimeTypes() const { return (this->_mimeTypes); }
bool					Response::getCgi() const { return (this->_isCGI); }
std::string const & 	Response::getStatusMessage(std::string const& code) const { return (this->_statusMessages.find(code)->second); }
std::string const &		Response::getMimeType(std::string const& extension) const { return (this->_mimeTypes.find(extension)->second); }
std::string const &		Response::getStatusCode() const { return (this->_statusCode); }
std::string const &		Response::getVersion() const { return (this->_version); }
std::string const &		Response::getStatusLine() const { return (this->_statusLine); }
std::string const &		Response::getHeaders(void) const { return (this->_headers); }
std::string const &		Response::getBodyContent() const { return (this->_bodyContent); }
std::string const &		Response::getResponse() const {	return (this->_response); }
std::string const &		Response::getTargetFinalPath() const { return (this->_targetFinalPath); }


void	Response::setStatusMessages(std::string const& filename) { this->_statusMessages = this->_initFile(filename); }
void	Response::setStatusCode(std::string const& statusCode) { this->_statusCode = statusCode; }
void	Response::setVersion(std::string const& version) { this->_version = version; }
void	Response::setMimeTypes(std::string const& filename) { this->_mimeTypes = this->_initFile(filename); }
void	Response::setStatusLine(std::string const& statusLine) { this->_statusLine = statusLine; }
void	Response::setBodyContent(std::string const & body) { this->_bodyContent = body; }
void	Response::setHeaders(std::string const& headers) { this->_headers = headers; }
void	Response::setResponse(std::string const& response) { this->_response = response; }
void	Response::setTargetFinalPath(std::string const& targetFinalPath) { this->_targetFinalPath = targetFinalPath; }
void	Response::setCgi(bool const& isCGI) { this->_isCGI = isCGI; }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::



void	Response::_buildStatusLine() {
	
	std::stringstream line;

	line << this->_request.getVersion() + " ";
	line << this->_statusCode + " ";
	line << this->getStatusMessage(this->_statusCode);
	line << CRLF;
	
	this->setStatusLine(line.str());
}

void	Response::_buildHeaders() {

	std::stringstream	headers;
	
	headers << "Content-Type: " << "text/html" << CRLF;
	headers << "Content-Length: " << this->_bodyContent.size() << CRLF;
	headers << CRLF;

	this->setHeaders(headers.str());
}

void	Response::_buildBody() {
	
	std::stringstream 	body;
	
	body	<< "<!DOCTYPE html>\n";
//			<< "<html>\n"
//			<< "<head>\n"
//			<< "<title>Page Title</title>\n"
//			<< "</head>\n"
//			<< "<body>\n"
//			<< "\n"
//			<< "<h1>This is a Heading</h1>\n"
//			<< "<p>This is a paragraph.</p>\n"
//			<< "\n"
//			<< "</body>\n"
//			<< "</html>\n";

	this->setBodyContent(body.str());
}

void	Response::buildResponse() {

	std::stringstream	res;

	this->_buildStatusLine();
	this->_buildBody();
	this->_buildHeaders();

	res << this->getStatusLine();
	res << this->getHeaders();
	res << this->getBodyContent();
	
	this->_response = res.str();
}

std::string Response::_trim(std::string const& str)
{
    const std::string ws = " \n\r\t\f\v";
    
    size_t start = str.find_first_not_of(ws);
    if (start == std::string::npos)
        return ""; // no content except whitespace

    size_t end = str.find_last_not_of(ws);
    
    return str.substr(start, end-start+1);
}

t_lexicon Response::_initFile(std::string const& filename) {

	std::ifstream	file(filename.c_str());

	if (!file.is_open())
		throw std::runtime_error("Could not open " +  filename + " file");

	t_lexicon		fileLexicon;
	std::string		line;
	
	while (std::getline(file, line) && !file.eof())
	{

		if (line.empty())
			continue;

		if (line.find_first_of("{}") != std::string::npos)
			continue;
		
		std::string			key;
		std::string			value;
		std::stringstream 	sstream(line);
		
		key.clear();
		value.clear();

		sstream >> key;
		std::getline(sstream, value, ';');
		value = this->_trim(value);
		fileLexicon[key] = value;
	}
	
	file.close();
	return (fileLexicon);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::OUTPUT OPERATOR OVERLOAD::

std::ostream& operator<<(std::ostream& o, Response const& rhs) {
	o << rhs.getResponse();
	return (o);
}
	