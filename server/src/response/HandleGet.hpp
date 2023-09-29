#ifndef HANDLEGET_HPP
# define HANDLEGET_HPP

# include "AHandler.hpp"

class HandleGet : public AHandler {

	public:
		HandleGet(Request const&, ServerContext const&);
		HandleGet(Response*);
		HandleGet(HandleGet const&);
		virtual ~HandleGet();
		HandleGet& operator=(HandleGet const&);

		void	handle();

	private:
};
#endif