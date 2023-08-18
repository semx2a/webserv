#include "../inc/Config.hpp"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::CONSTRUCTORS / DESTRUCTORS

Config::Config() {

	ports.push_back(8080);
	ports.push_back(8081);
	ports.push_back(8082);
	ports.push_back(8083);
	ports.push_back(8084);
}

Config::Config(std::string const& conf_file) {

	(void) conf_file;
	ports.push_back(8080);
	ports.push_back(8081);
	ports.push_back(8082);
	ports.push_back(8083);
	ports.push_back(8084);
}

Config::Config(Config const& rhs) { *this = rhs; }

Config& Config::operator=(Config const& rhs) { 

	if(this != &rhs) {

	}
	return *this;
}

Config::~Config() {
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::GETTERS / SETTERS

std::vector<int>&			Config::getPorts() { return ports; }

std::string const&			Config::get(std::string const& key) const { return data.at(key); }

std::vector<std::string>&	Config::getArray(std::string const& key) const { return arrayData.at(key); }

Config*						Config::getBlock(std::string const& key) const { return blockData.at(key); }

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::METHODS

void parse(const std::string& filename) {

	std::ifstream	file(filename.c_str());
	std::string		line;

	while (std::getline(file, line)) {

		std::istringstream	is_line(line);
		std::string			key;

		if (std::getline(is_line, key, ' ')) {
			std::string value;

			if (std::getline(is_line, value)) {
				if (value.find('{') != std::string::npos) {
					// This is a block, so we need to parse it recursively
					std::string blockFilename = value.substr(0, value.find('{'));
					blockData[key] = new Config(blockFilename);
				} else if (value.find(',') != std::string::npos) {
					// This is an array, so we split it into multiple values
					std::istringstream is_value(value);
					std::string arrayValue;
					std::vector<std::string> values;
					while (std::getline(is_value, arrayValue, ',')) {
						values.push_back(arrayValue);
					}
					arrayData[key] = values;
				} else {

					// This is a simple key-value pair
					data[key] = value;
				}
			}
		}
	}
}
};

