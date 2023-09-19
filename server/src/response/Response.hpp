/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 11:35:17 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/18 18:16:20 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>

# include "macros.hpp"

class Response {

	public:

		Response();
		Response(Response const& rhs);
		~Response();
		Response& operator=(Response const& rhs);

		std::string const&					getStatusLine(void) const;
		std::map<int, std::string> const&	getStatusCodes(void) const;
		std::vector<std::string> const&		getErrorPages(void) const;
		std::string const&					getResponse(void) const;

		void	setStatusLine(std::string const& statusLine);
		void	setStatusCodes(void);
		void	setErrorPages(std::vector<std::string> const& errorPages);
		void	setResponse(std::string const& response);
		

		void	buildResponse(void);

	private:

		std::string					_statusLine;
		std::string					_version;
		std::map<int, std::string>	_statusCodes;
		
		std::vector<std::string>	_errorPages;
		
		std::string					_response;

		

};

std::ostream& operator<<(std::ostream& o, Response const& rhs);

#endif