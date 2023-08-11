/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 17:07:53 by seozcan           #+#    #+#             */
/*   Updated: 2023/08/10 16:31:54 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <iostream>
# include <map>
# include <vector>
# include <sstream>

# include "display.hpp"
# include "Epoll.hpp"
# include "print.hpp"


class Request {
	
	public:

		Request(void);
		Request(Request const &src);
		Request(std::string const);
		~Request(void);

		Request &	operator=(Request const &rhs);

		void	setMethod(const std::string);
		void	setTarget(const std::string);
		void	setQuery(const std::string);
		void	setVersion(const std::string);
		void	setHeaders(const std::map<std::string, std::vector<std::string> >);
		void	setBody(const std::vector<char>);

		const std::string										getMethod(void) const;
		const std::string										getTarget(void) const;
		const std::string										getQuery(void) const;
		const std::string										getVersion(void) const;
		const std::map<std::string, std::vector<std::string> >	getHeaders(void) const;
		const std::vector<char>									getBody(void) const;

		void	parser(std::string const);
		void	parseRequestLine(std::istringstream&);
		void	parseHeaders(std::istringstream&);
		void	parseBody(std::istringstream&);

		//void	printRequestHeaders(void);

	private:
	
		std::string												_method;
		std::string												_target;
		std::string												_query;
		std::string												_version;
		std::map<std::string, std::vector<std::string> >		_headers;
		std::vector<char>										_body;

};

std::ostream &	operator<<(std::ostream & o, Request const & r);

#endif