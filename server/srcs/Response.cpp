/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 10:34:31 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/18 11:43:03 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

Response::Response()
{
}

Response::Response(Response const& rhs)
{
	*this = rhs;
}

Response::~Response()
{
}

Response& Response::operator=(Response const& rhs)
{
	if (this != &rhs)
	{
		this->_response = rhs._response;
	}
	return (*this);
}

std::string const&	Response::getResponse() const
{
	return (this->_response);
}

void				Response::setResponse(std::string const& response)
{
	this->_response = response;
}

std::ostream& operator<<(std::ostream& o, Response const& rhs)
{
	o << rhs.getResponse();
	return (o);
}
