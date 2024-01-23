#ifndef STATUS_LINE_HPP
# define STATUS_LINE_HPP

# include "ARespComponent.hpp"
# include "StatusCodes.hpp"

class StatusLine : public ARespComponent {
	
	public:
		StatusLine(void);
		StatusLine(std::string const&, std::string const&);
		StatusLine(StatusLine const &);
		~StatusLine();
	
		StatusLine &		operator=(StatusLine const &);

		void				setStatusCode(std::string const&);
		void				setReasonPhrase(std::string const &);

		std::string const&	statusCode(void) const;
		std::string const&	reasonPhrase(void) const;

		void		build(void);
		void		build(std::string const&, std::string const&);

	private:
	 	std::string _statusCode;
		std::string	_reasonPhrase;
};

#endif