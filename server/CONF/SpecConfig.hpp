#ifndef SPEC_CONFIG_HPP
# define SPEC_CONFIG_HPP

#include "CommonConfig.hpp"

class SpecConfig : public CommonConfig {

	public:
		SpecConfig();
		SpecConfig(SpecConfig const& rhs);
		SpecConfig& operator=(SpecConfig const& rhs);
		virtual ~SpecConfig();

		std::map<std::string, int> const &		getListenIpPort(void) const;
		std::vector<std::string> const &		getServerNames(void) const;

		void	setListIpPort(std::string const& line);

	private:
		std::map<std::string, int> 	_listenIpPort; // <IP, port>
		std::vector<std::string>	_serverNames;
		


};

#endif