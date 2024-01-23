#ifndef HTTP_STATUS_HPP
# define HTTP_STATUS_HPP

# include <iostream>
# include <string>


class HttpStatus : public std::exception {

	public:
		HttpStatus(void);
		HttpStatus(HttpStatus const&);
		HttpStatus(std::string const&);
		virtual ~HttpStatus() throw();

		HttpStatus &				operator=(HttpStatus const&);

		static std::string const&	statusCode(void);
		static void 				setStatusCode(std::string const& statusCode); 
		
		virtual const char*			what() const throw();

	private:
		static std::string _statusCode;
};

#endif
