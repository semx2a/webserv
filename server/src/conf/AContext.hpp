#ifndef ACONTEXT_HPP
# define ACONTEXT_HPP

#include <iomanip>
#include <map>
#include <vector>

#include "utl.hpp"

class AContext {

	public:
		AContext();
		AContext(AContext const&);
		AContext& operator=(AContext const& rhs);
		virtual ~AContext();

		std::string const&							autoindex(void) const;
		size_t										maxBodySize(void) const;
		std::string const &							pwd(void) const;
		std::string const&							root(void) const;
		std::map<int, std::string> const&			errorPages(void) const;
		std::vector<std::string> const&				index(void) const;
		std::vector<std::string> const&				authorizedMethods(void) const;
		std::string const&							uploadFolder(void) const;

		void	setAutoindex(std::string const& autoindex);
		void 	setMaxBodySize(size_t size);
		void	setPwd(std::string const& pwd);
		void	setRoot(std::string const& root);
		void	setUploadFolder(std::string const& uploadFolder);
		void	addErrorPage(int code, std::string const& path);
		void	setErrorPages(std::map<int, std::string> const& errorPages);
		void	addIndex(std::string const& index);
		void	setIndex(std::vector<std::string> const& index);
		void	addAuthorizedMethod(std::string const& method);
		void	setAuthorizedMethods(std::vector<std::string> const& authorizedMethods);

		virtual void	setDefaults(void);

	protected:
		std::string							_autoindex;
		size_t								_maxBodySize;
		std::string							_pwd;
		std::string							_root;
		std::map<int, std::string>			_errorPages;		
		std::vector<std::string>			_index;
		std::vector<std::string>			_authorizedMethods;
		std::string 						_uploadFolder;

};

#endif
