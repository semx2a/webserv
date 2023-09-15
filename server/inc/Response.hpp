/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 11:35:17 by seozcan           #+#    #+#             */
/*   Updated: 2023/09/15 16:58:26 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>

# include "Macros.hpp"
# include "Request.hpp"
# include "Config.hpp"

typedef enum e_HTTPstatus {
	CONTINUE						= 100,
	SWITCHING_PROTOCOLS				= 101,
	OK								= 200,
	CREATED							= 201,
	ACCEPTED						= 202,
	NON_AUTHORITATIVE_INFORMATION 	= 203,
	NO_CONTENT 						= 204,
	RESET_CONTENT					= 205,
	PARTIAL_CONTENT					= 206,
	MULTIPLE_CHOICES				= 300,
	MOVED_PERMANENTLY				= 301,
	FOUND							= 302,
	SEE_OTHER						= 303,
	NOT_MODIFIED					= 304,
	USE_PROXY						= 305,
	TEMPORARY_REDIRECT				= 307,
	BAD_REQUEST						= 400,
	UNAUTHORIZED					= 401,
	PAYMENT_REQUIRED				= 402,
	FORBIDDEN						= 403,
	NOT_FOUND						= 404,
	METHOD_NOT_ALLOWED				= 405,
	NOT_ACCEPTABLE					= 406,
	PROXY_AUTHENTICATION_REQUIRED	= 407,
	REQUEST_TIMEOUT					= 408,
	CONFLICT						= 409,
	GONE							= 410,
	LENGTH_REQUIRED					= 411,
	PRECONDITION_FAILED				= 412,
	PAYLOAD_TOO_LARGE				= 413,
	REQUEST_URI_TOO_LONG			= 414,
	UNSUPPORTED_MEDIA_TYPE			= 415,
	REQUESTED_RANGE_NOT_SATISFIABLE	= 416,
	EXCEPTION_FAILED				= 417,
	INTERNAL_SERVER_ERROR			= 500,
	NOT_IMPLEMENTED					= 501,
	BAD_GATEWAY						= 502,
	SERVICE_UNAVAILABLE				= 503,
	GATEWAY_TIMEOUT					= 504,
	HTTP_VERSION_NOT_SUPPORTE		= 505
}	t_HTTPstatus;

class Response {

	public:

		Response();
		Response(Response const& rhs);
		~Response();
		Response& operator=(Response const& rhs);

		std::string const&					getStatusLine(void) const;
		std::map<int, std::string> const&	getStatusCodes(void) const;
		std::string const&					getStatusCode(int code) const;
		std::vector<std::string> const&		getErrorPages(void) const;
		std::string const&					getResponse(void) const;

		void	setStatusLine(std::string const& statusLine);
		void	setStatusCodes(void);
		void	setErrorPages(std::vector<std::string> const& errorPages);
		void	setResponse(std::string const& response);
		
		void	buildResponse(Request const&, Config const&, int const&);

	private:

		std::string					_statusLine;
		std::string					_version;
		std::map<int, std::string>	_statusCodes;
		
		std::vector<std::string>	_errorPages;
		
		std::string					_response;

		

};

std::ostream& operator<<(std::ostream& o, Response const& rhs);

#endif