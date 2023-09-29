#ifndef RESPONSE_HANDLER_HPP
# define RESPONSE_HANDLER_HPP

//# define DEBUG_RESPONSE_HANDLER

#include "Response.hpp"

class ResponseHandler {

	public:
		
		ResponseHandler(Response* response);
		ResponseHandler(ResponseHandler const& rhs);
		~ResponseHandler();
		ResponseHandler& operator=(ResponseHandler const& rhs);

		Response*	getResponse() const;

		void		setResponse(Response* response);

		void	handleResponse();

	private:
		ResponseHandler();

		Response*	_response;

		void		_checkTarget();
		void 		_handleAutoIndex();
		void		_handleGet();
		void		_handlePost();
		void		_handleDelete();
		void		_assignIndex(std::vector<std::string> const&);



};

#endif