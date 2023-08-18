/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 11:35:17 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/18 11:40:22 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>

class Response {

	public:

		Response();
		Response(Response const& rhs);
		~Response();
		Response& operator=(Response const& rhs);

		std::string const&	getResponse() const;
		void				setResponse(std::string const& response);

	private:

		std::string			_response;

};

std::ostream& operator<<(std::ostream& o, Response const& rhs);

#endif