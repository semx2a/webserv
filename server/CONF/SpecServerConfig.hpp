#ifndef SPECSERVERCONFIG_HPP
# define SPECSERVERCONFIG_HPP

#include "CommonServerConfig.hpp"

class SpecServerConfig : public CommonServerConfig {

	public:
		SpecServerConfig(std::string const& SpecServerConfig_file);
		SpecServerConfig(SpecServerConfig const&);
		SpecServerConfig& operator=(SpecServerConfig const& rhs);
		virtual ~SpecServerConfig();

		std::map<std::string, int>	getListenIpPort(void) const;
		std::vector<std::string>	getServerNames(void) const;

	private:
		SpecServerConfig();

		std::map<std::string, int> 	_listenIpPort; // <IP, port>
		std::vector<std::string>	_serverNames;
		


};

#endif