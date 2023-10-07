#include "ResponseContext.hpp"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

ResponseContext::ResponseContext() : _serverContext(ServerContext()) {}

ResponseContext::ResponseContext(ServerContext const& sc) : _serverContext(sc) {}

ResponseContext::ResponseContext(ResponseContext const & rhs) : _serverContext(rhs._serverContext) {
	
	*this = rhs;
}

ResponseContext::~ResponseContext() {}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: OPERATORS::

ResponseContext &		ResponseContext::operator=(ResponseContext const & rhs) {

	if (this != &rhs) {
		// TODO
	}
	return *this;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

ServerContext const&	ResponseContext::serverContext() const { return _serverContext; }


// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: METHODS::

