#ifndef COMMON_CONFIG_HPP
# define COMMON_CONFIG_HPP

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class CommonConfig {

	public:
		CommonConfig();
		CommonConfig(CommonConfig const&);
		CommonConfig& operator=(CommonConfig const& rhs);
		virtual ~CommonConfig();

		size_t										getClientMaxBodySize() const;
		std::map<int, std::string> const &			getErrorPage() const;
		std::vector<std::string> const&				getIndex() const;
		std::string const&							getRoot() const;
		std::map<std::string, std::string> const&	getLocations() const;
		bool										getAutoindex() const;
		std::vector<std::string> const&				getAuthorizedMethods() const;

		void		setClientMaxBodySize(std::string const& line);
		void		setErrorPage(std::string const& line);
		void		setIndex(std::string const& line);
		void		setRoot(std::string const& line);
		void		setAutoindex(std::string const& line);
		void		setAuthorizedMethods(std::string const& line);
		void		setLocation(std::stringstream& stream);

	private:
		size_t									_clientMaxBodySize;
		std::map<int, std::string>				_errorPages;
		std::vector<std::string>				_index;
		std::string								_root;
		std::map<std::string, std::string>		_locations;
		bool									_autoindex;
		std::vector<std::string>				_authorizedMethods;

};

#endif
