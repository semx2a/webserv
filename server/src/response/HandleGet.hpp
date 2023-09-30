#ifndef HANDLEGET_HPP
# define HANDLEGET_HPP

# include "AHandler.hpp"

class HandleGet : public AHandler {

	public:
		HandleGet(Request const&, ServerContext const&);
		HandleGet(HandleGet const&);
		virtual ~HandleGet();
		HandleGet& operator=(HandleGet const&);

		void	handle();

	private:
		void 		_handleAutoIndex();
		void		_assignIndex(std::vector<std::string> const&);
};

#endif