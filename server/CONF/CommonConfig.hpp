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
		std::string const&							getErrorPage() const;
		std::string const&							getIndex() const;
		std::map<std::string, std::string> const&	getLocations() const;
		bool										getAutoindex() const;
		std::vector<std::string> const&				getLimitExcept() const;

		void		setClientMaxBodySize(size_t clientMaxBodySize);
		void		setErrorPage(std::string const& errorPage);
		void		setIndex(std::string const& index);
		void		setLocation(std::string const& location, std::string const& path);
		void		setAutoindex(bool autoindex);
		void		setLimitExcept(std::vector<std::string> const& limit_except);

	private:
		size_t									_clientMaxBodySize;
		std::string								_errorPage;
		std::string								_index;
		std::map<std::string, std::string>		_locations;
		bool									_autoindex;
		std::vector<std::string>				_limit_except;

};

#endif
