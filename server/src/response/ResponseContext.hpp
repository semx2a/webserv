#ifndef RESPONSECONTEXT_HPP
# define RESPONSECONTEXT_HPP

# include "ServerContext.hpp"

class ResponseContext {

	public:

		ResponseContext();
		ResponseContext(ServerContext const&);
		ResponseContext(ResponseContext const&);
		ResponseContext(ServerContext const&);
		~ResponseContext();
		ResponseContext& operator=(ResponseContext const&);

		// :::::::::::::::::::::::::::: ACCESSORS
		ServerContext const&	serverContext() const;

	private:
		ServerContext const& _serverContext;
};

#endif