#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "HandleGet.hpp"
#include "StatusLine.hpp"
#include "Headers.hpp"
#include "Body.hpp"

class ResponseHandler;
class ResponseBuilder;

class Response {

	public:

		//Response();
		Response(Response const& rhs);
		Response(Request const&, ServerContext const&);
		~Response();
		Response& operator=(Response const& rhs);


		std::string const&		getStatusLine() const;
		std::string const&		getHeaders(void) const;
		std::string const&		getBody() const;
		std::string const&		getResponseStr() const;


		void	setStatusLine(std::string const&);
		void	setHeaders(std::string const&);
		void	setBody(std::string const&);
		void	setResponseStr(std::string const&);
		
	private:

		AHandler *		_method;
		StatusLine		_statusLine;
		Headers			_headers;
		Body			_body;

		std::string		_responseStr;

};

std::ostream& operator<<(std::ostream& o, Response const& rhs);

#endif