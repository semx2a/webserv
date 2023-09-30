#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "HandleGet.hpp"

class Response {

	public:

		Response();
		Response(Response const& rhs);
		~Response();
		Response& operator=(Response const& rhs);


		AHandler*				getMethodHandler() const;
		std::string const&		getStatusLine() const;
		std::string const&		getHeaders(void) const;
		std::string const&		getBody() const;
		std::string const&		getResponseStr() const;


		void	setMethodHandler(AHandler*);
		void	setStatusLine(std::string const&);
		void	setHeaders(std::string const&);
		void	setBody(std::string const&);
		void	setResponseStr(std::string const&);
		
	private:

		AHandler *		_methodHandler;
		std::string		_statusLine;
		std::string		_headers;
		std::string		_body;

		std::string		_responseStr;

};

std::ostream& operator<<(std::ostream& o, Response const& rhs);

#endif