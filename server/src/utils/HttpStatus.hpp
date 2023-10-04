#ifndef HTTP_STATUS_HPP
# define HTTP_STATUS_HPP

# include <iostream>
# include <string>


class HttpStatus : public std::exception {

	private:
		static std::string _statusCode;

	public:
		HttpStatus(std::string const& statusCode);
		virtual ~HttpStatus() throw();
		
		static std::string const&	statusCode();
		virtual const char*			what() const throw();
};

#endif
