#ifndef RESPONSE_HANDLER_HPP
# define RESPONSE_HANDLER_HPP

# define DEBUG_RESPONSE_HANDLER

#include "Response.hpp"

typedef std::map<std::string, LocationContext>::const_iterator	t_locationIterator;
typedef std::map<std::string, ServerContext>::const_iterator	t_serverIterator;

class Response;

class ResponseHandler {

	public:
		
		ResponseHandler(Response*);
		ResponseHandler(ResponseHandler const&);
		~ResponseHandler();
		ResponseHandler& operator=(ResponseHandler const&);

		Response*	getResponse() const;

		void		setResponse(Response*);

		void	handleResponse();

	private:
		ResponseHandler();

		Response*	_response;

		void		_setPath();
		void 		_handleAutoIndex();
		void		_handleGet();
		void		_handlePost();
		void		_handleDelete();
		void		_assignIndex(std::vector<std::string> const&);



};

#endif