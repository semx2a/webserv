#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "extern_libs.hpp"

class Config {

	public:
		Config ();
		Config (std::string const& config_file);
		Config (Config const&);
		Config& operator= (Config const& rhs);
		~Config();

		std::vector <int>&	getPorts ();

	private:
		std::vector <int>	ports;
		
};

#endif
