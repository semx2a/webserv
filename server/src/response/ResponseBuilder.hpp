#ifndef RESPONSE_BUILD_HPP
# define RESPONSE_BUILD_HPP

#include "Response.hpp"

class Response;

class ResponseBuilder {

	public:
		ResponseBuilder(Response* response);
		ResponseBuilder(ResponseBuilder const&);
		~ResponseBuilder();
		ResponseBuilder& operator=(ResponseBuilder const&);
		
		void	buildResponse();

	private:
		ResponseBuilder();

		Response*	_response;
		
		void		_buildStatusLine();
		void		_buildHeaders();
		void		_buildBody();

};

#endif