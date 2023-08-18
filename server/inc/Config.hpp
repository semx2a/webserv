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
		std::string	const&			get(const std::string& key) const; 
		std::vector<std::string>&	getArray(const std::string& key) const; 

	    Config* getBlock(const std::string& key) const;


	private:
		std::vector<int>								ports;
		std::map<std::string, std::string>				data;
		std::map<std::string, std::vector<std::string>>	arrayData;
		std::map<std::string, Config*>					blockData;
		
};

#endif
