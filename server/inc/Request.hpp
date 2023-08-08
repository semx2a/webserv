/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 17:07:53 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/08 18:48:00 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <iostream>
# include <map>
# include <vector>
# include "Epoll.hpp"

class Request {
	
	public:

		Request(void);
		Request(Request const &src);
		Request(std::string const);
		~Request(void);

		Request &	operator=(Request const &rhs);

		void	setMethod(const std::string);
		void	setTarget(const std::string);
		void	setVersion(const std::string);
		void	setQuery(const std::string);
		void	setBody(const std::vector<char>);
		void	setHeaders(const std::map<std::string, std::vector<std::string> >);

		const std::string										getMethod(void) const;
		const std::string										getTarget(void) const;
		const std::string										getVersion(void) const;
		const std::string										getQuery(void) const;
		const std::vector<char>									getBody(void) const;
		const std::map<std::string, std::vector<std::string> >	getHeaders(void) const;

		void	parse(std::string const);

	private:
	
		std::string												_method;
		std::string												_target;
		std::string												_version;
		std::string												_query;
		std::vector<char>										_body;
		std::map<std::string, std::vector<std::string> >		_headers;

};

std::ostream &	operator<<(std::ostream & o, Request const & r);

#endif