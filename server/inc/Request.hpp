/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 17:07:53 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/06 19:05:12 by seozcan          ###   ########.fr       */
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
		~Request(void);

		Request &	operator=(Request const &rhs);

		void	setMethod(const std::string);
		void	setPath(const std::string);
		void	setHTTP(const std::string);
		void	setQuery(const std::string);
		void	setBody(const std::vector<char>);
		void	setHeader(const std::map<std::string, std::vector<std::string> >);

		const std::string										getMethod(void) const;
		const std::string										getPath(void) const;
		const std::string										getHTTP(void) const;
		const std::string										getQuery(void) const;
		const std::vector<char>									getBody(void) const;
		const std::map<std::string, std::vector<std::string> >	getHeader(void) const;

	private:
	
		std::string												_method;
		std::string												_path;
		std::string												_httpVersion;
		std::string												_query;
		std::vector<char>										_body;
		std::map<std::string, std::vector<std::string> >		_header;

};

std::ostream &	operator<<(std::ostream & o, Request const & r);

#endif