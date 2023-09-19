#include <iostream>
#include "CommonConfig.hpp"
#include "Config.hpp"
#include "Parser.hpp"
#include "SpecConfig.hpp"
#include "print.hpp"

void	DisplayParserContents(Parser const& parser) {

	std::cout << std::endl;
	std::cout << HIORANGE << "--- Parser ---" << NO_COLOR << std::endl;
	std::cout << HIPURPLE << "linesRead: " << NO_COLOR << parser.getLinesRead() << std::endl;
	std::cout << HIPURPLE << "confFilename: " << NO_COLOR << parser.getConfFileName() << std::endl;
}

void	DisplayCommonConfig(CommonConfig const& config) {

	std::cout << std::endl;
	std::cout << HIORANGE << "--- Config ---" << NO_COLOR << std::endl;
	std::cout << HIPURPLE << "clientMaxBodySize: " << NO_COLOR << config.getClientMaxBodySize() << std::endl;
	std::cout << HIPURPLE << "errorPages: " << NO_COLOR << std::endl;
	print_map(config.getErrorPages());
}

void	DisplaySpecConfig(SpecConfig& specConfig) {

	std::cout << "\033[1;31m--- SpecConfig ---\033[0m" << std::endl;
	(void)specConfig;
}

int	main() {

	Config 					config;
	//Config 				config("path to config file");
	Parser					parser = config.getParser();
	CommonConfig			commonConfig = parser.getCommonConfig();
	std::vector<SpecConfig>	specConfigs = parser.getSpecConfigs();

	DisplayParserContents(parser);
	DisplayCommonConfig(commonConfig);

	return 0;
}
