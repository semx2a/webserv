#include "Response.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Response::Response() : _methodHandler(NULL) {

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

AHandler*				Response::getMethodHandler() const { return (this->_methodHandler); }

std::string const &		Response::getStatusLine() const { return (this->_statusLine); }
std::string const &		Response::getHeaders(void) const { return (this->_headers); }
std::string const &		Response::getBody() const { return (this->_body); }

std::string const &		Response::getResponseStr() const { return (this->_responseStr); }


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: SETTERS::

void	Response::setMethodHandler(AHandler* resHandler) { this->_methodHandler = resHandler; }

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
	