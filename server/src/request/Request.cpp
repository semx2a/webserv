#include "Request.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Request::Request(void) : _method(), _target(), _query(), _version(), _headers(), _body() {}

Request::Request(Request const &src) {
	if (this != &src)
		*this = src;
}

Request::~Request() {}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMPARISON OPERATORS::

Request &Request::operator=(Request const &rhs)
{

	if (this != &rhs)
	{
		this->setMethod(rhs.method());
		this->setTarget(rhs.target());
		this->setVersion(rhs.version());
		this->setQuery(rhs.query());
		this->setBody(rhs.body());
		this->setHeaders(rhs.headers());
	}
	return *this;
}


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

void Request::setMethod(const std::string method) { this->_method = method; }
void Request::setTarget(const std::string target) { this->_target = target; }
void Request::setVersion(const std::string version) { this->_version = version; }
void Request::setQuery(const std::string query) { this->_query = query; }
void Request::setBody(const std::vector<char> body) { this->_body = body; }
void Request::setHeaders(const std::map<std::string, std::string> headers) { this->_headers = headers; }


std::string const & 		Request::method(void) const { return this->_method; }
std::string const & 		Request::target(void) const { return this->_target; }
std::string const & 		Request::version(void) const { return this->_version; }
std::string const & 		Request::query(void) const { return this->_query; }
std::vector<char> const & 	Request::body(void) const { return this->_body; }
std::string const & 		Request::header(std::string const & key) const { return this->_headers.find(key)->second; }
t_headers const &		 	Request::headers(void) const { return this->_headers; }


// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::: MEMBER FUNCTIONS::

void Request::_parseBody(std::vector<char> const& str_vec) {
	
	int pos = utl::find_last_occurrence(str_vec, "\r\n\r\n");
	
	this->_body.assign(str_vec.begin() + (pos + 1), str_vec.end());
}

void Request::_parseHeaders(std::istringstream &stream) {

	std::string 				line;
	std::vector<std::string>	headers_vec;

	if (!this->_headers.empty())
		this->_headers.clear();
	
	std::getline(stream, line);
	while (std::getline(stream, line)) {

		if (line.empty() || line == "\r")
			break;
		line = utl::trim(line);
		headers_vec.push_back(line);
	}
	
	for (std::vector<std::string>::iterator it = headers_vec.begin(); it != headers_vec.end(); it++)
	{
		std::istringstream	hd_line(*it);
		std::string			hd_key;
		std::string 		hd_values;
		
		hd_key.erase(); 	
		hd_values.erase();
		std::getline(hd_line, hd_key, ':');
		std::getline(hd_line, hd_values);

		if (hd_key.empty())
			throw HttpStatus("400");
		if (hd_values.empty())
			hd_values = "";
		
		hd_values = utl::trim(hd_values);
 		this->_headers[hd_key] = hd_values;
	}
	
	if (this->headers().empty())
		throw HttpStatus("400");
}

void Request::_parseRequestLine(std::istringstream &stream) {
	
	stream >> this->_method >> this->_target >> this->_version;

	if (this->_method.empty() || this->_target.empty() || this->_version.empty())
		throw HttpStatus("400");
	if (this->_version != "HTTP/1.1")
		throw HttpStatus("505");
	if (this->_target.find('?', 0) != std::string::npos) {

		std::string newTarget;

		newTarget = std::string(this->_target.begin(), this->_target.begin() + this->_target.find('?', 0));
		this->setQuery(std::string(this->_target.begin() + this->_target.find('?', 0) + 1, this->_target.end()));
		this->setTarget(newTarget);
	}
}

void Request::parser(std::vector<char> const& str_vec) {
	
	std::string			str(str_vec.begin(), str_vec.end());
	std::istringstream	stream(str);

	if (str.empty())
		return ;
	this->_parseRequestLine(stream);
	this->_parseHeaders(stream);
	if (!stream.eof())
		this->_parseBody(str_vec);

	#ifdef DEBUG_REQUEST
		std::cout << "[DEBUG]" << *this << std::endl;
	#endif

}

// ::::::::::::::::::::::::::::::::::::::::::::::: OUTPUT OPERATOR OVERLOADING::

std::ostream &operator<<(std::ostream &o, Request const &r) {

	o << "  " << utl::str_of(60, ":") << std::endl << std::endl;
	o << "method: " 	<< r.method() << std::endl;
	o << "target: "		<< r.target() << std::endl;
	o << "version: "	<< r.version() << std::endl;
	o << "query: "		<< r.query() << std::endl;
	o << "headers: "	<< utl::print_map(r.headers()) << std::endl;
	o << "body: " 		<< utl::print_vector(r.body()) << std::endl;

	return o;
}
