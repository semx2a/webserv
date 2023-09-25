#ifndef ACONTEXT_HPP
# define ACONTEXT_HPP

#include <map>
#include <vector>

#include "print.hpp"

class AContext {

	public:
		AContext();
		AContext(AContext const&);
		AContext& operator=(AContext const& rhs);
		virtual ~AContext();

		bool										getAutoindex() const;
		bool										getCgi() const;
		size_t										getMaxBodySize() const;
		std::string const&							getRoot() const;
		std::map<int, std::string> const&			getErrorPages() const;
		std::vector<std::string> const&				getIndex() const;
		std::vector<std::string> const&				getAuthorizedMethods() const;

		void	setAutoindex(bool autoindex);
		void	setCgi(bool cgi);
		void 	setMaxBodySize(size_t size);
		void	setRoot(std::string const& root);
		void	setListen(std::string const& ip, int port);
		void	addErrorPage(int code, std::string const& path);
		void	setErrorPages(std::map<int, std::string> const& errorPages);
		void	setIndex(std::vector<std::string> const& index);
		void	setAuthorizedMethods(std::vector<std::string> const& authorizedMethods);

		virtual void	setDefaults(void);

	protected:
		bool								_autoindex;
		bool								_cgi;
		size_t								_maxBodySize;
		std::string							_root;
		std::map<int, std::string>			_errorPages;		
		std::vector<std::string>			_index;
		std::vector<std::string>			_authorizedMethods;

};

#endif
