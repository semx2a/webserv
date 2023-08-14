/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 15:23:20 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/14 17:22:17 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <exception>
# include <string>

class ServerException : public std::exception {
	
	public:
	
	

	private:

		int			_errorCode;
		std::string	_errorMsg;
};

#endif