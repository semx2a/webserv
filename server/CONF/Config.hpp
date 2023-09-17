#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class Config {

	public:
		Config();
		Config(std::string const& config_file);
		Config(Config const&);
		Config& operator=(Config const& rhs);
		~Config();

		std::vector<int>&			getPorts();

	private:
		std::vector<int>			_ports;
		
		
};

#endif
