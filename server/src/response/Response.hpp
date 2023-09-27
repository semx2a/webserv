/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 11:35:17 by seozcan           #+#    #+#             */
/*   Updated: 2023/09/27 18:31:25 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <fstream>

# include "macros.hpp"
# include "Request.hpp"
# include "ServerContext.hpp"

typedef std::map<std::string, std::string>		t_lexicon;

class Response {

	public:

		Response();
		Response(Response const& rhs);
		Response(Request const&, ServerContext const&);
		~Response();
		Response& operator=(Response const& rhs);

		t_lexicon				getStatusCodes() const;
		t_lexicon				getMimeTypes() const;
		std::string const&		getStatusCode(std::string const &) const;
		std::string const&		getMimeType(std::string const&) const;
		std::string const&		getStatusLine() const;
		std::string const&		getVersion() const;
		std::string const&		getResponse() const;
		ServerContext const&	getServerContext() const;
		Request const&			getRequest() const;

		void	setStatusCodes(std::string const&);
		void	setMimeTypes(std::string const&);
		void	setStatusLine(std::string const&);
		void	setVersion(std::string const&);
		void	setResponse(std::string const&);
		
		void	buildResponse();

	private:

		t_lexicon		_statusCodes;
		t_lexicon 		_mimeTypes;
		std::string		_statusLine;
		std::string		_version;
		std::string		_response;
		Request			_request;
		ServerContext	_serverContext;

		t_lexicon	_initFile(std::string const&);
};

std::ostream& operator<<(std::ostream& o, Response const& rhs);

#endif