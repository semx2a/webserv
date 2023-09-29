#include "Response.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

/* Response::Response() : 
			_statusCode("200"),
			_version("HTTP/1.1"),
			_isCGI(false),
			_path(""),
			_handler(this),
			_builder(this) {

	this->setStatusMessages("../neoserv/status.codes");
	this->setMimeTypes("../neoserv/mime.types");	

	this->_handler.handleResponse();
	this->_builder.buildResponse();

} */

Response::Response(Request const& request, ServerContext const& conf) : _request(request), _serverContext(conf) {

	//TODO
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
		//TODO
	}
	return (*this);
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

Request const &			Response::getRequest() const { return (this->_request); }
ServerContext const &	Response::getServerContext() const { return (this->_serverContext); }

StatusCodes const &		Response::getStatusCodes() const { return this->_statusCodes; }
MimeTypes const &		Response::getMimeTypes() const { return this->_mimeTypes; }

std::string const &		Response::getPath() const { return (this->_path); }

std::string const &		Response::getStatusLine() const { return (this->_statusLine); }
std::string const &		Response::getHeaders(void) const { return (this->_headers); }
std::string const &		Response::getBody() const { return (this->_body); }

std::string const &		Response::getResponseStr() const { return (this->_responseStr); }


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: SETTERS::

void	Response::setRequest(Request const& request) { this->_request = request; }
void	Response::setServerContext(ServerContext const& serverContext) { this->_serverContext = serverContext; }

void	Response::setStatusCodes(StatusCodes const& statusCodes) { this->_statusCodes = statusCodes; }
void	Response::setMimeTypes(MimeTypes const& mimeTypes) { this->_mimeTypes = mimeTypes; }

void	Response::setPath(std::string const& targetFinalPath) { this->_path = targetFinalPath; }

void	Response::setStatusLine(std::string const& statusLine) { this->_statusLine = statusLine; }
void	Response::setHeaders(std::string const& headers) { this->_headers = headers; }
void 	Response::setBody(std::string const& body) { this->_body = body; }
void	Response::setResponseStr(std::string const& responseStr) { this->_responseStr = responseStr; }


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::



//::::::::::::::::::::::::::::::::::::::::::::::::::::OUTPUT OPERATOR OVERLOAD::

std::ostream& operator<<(std::ostream& o, Response const& rhs) {
	o << rhs.getResponseStr();
	return (o);
}
	