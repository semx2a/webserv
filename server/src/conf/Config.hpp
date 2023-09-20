#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "Parser.hpp"

class Config {

	public:
		Config();
		Config(std::string const& config_file);
		Config(Config const&);
		Config& operator=(Config const& rhs);
		~Config();

		std::vector<int>&			getPorts();
		Parser const&				getParser() const;

	private:
		std::vector<int>			_ports;
		Parser						_parser;
};

#endif