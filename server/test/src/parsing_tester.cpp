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

//void	DisplayCommonConfig(CommonConfig const& config) {
//
//
//
//}
//
//void	DisplaySpecConfig(SpecConfig& config) {
//
//	std::cout << "\033[1;31m--- SpecConfig ---\033[0m" << std::endl;
//	std::cout << std::endl;
//	std::cout << HIORANGE << "--- Config ---" << NO_COLOR << std::endl;
//	std::cout << HIPURPLE << "clientMaxBodySize: " << NO_COLOR << config.getClientMaxBodySize() << std::endl;
//	std::cout << HIPURPLE << "errorPages: " << NO_COLOR << std::endl;
//	print_map(config.getErrorPages());	
//	(void)specConfig;
//}

int	main() {

	Config 							config;
//	Config 							config("path to config file");
	Parser const &					parser = config.getParser();
//	CommonConfig const &			commonConfig = parser.getCommonConfig();
//	std::vector<SpecConfig> const &	specConfigs = parser.getSpecConfigs();

	DisplayParserContents(parser);
//	std::cout << commonConfig << std::endl;
//	std::cout << print_vector(specConfigs) << std::endl;
	std::cout << parser.getCommonConfig() << std::endl;
	std::cout << print_vector(parser.getSpecConfigs()) << std::endl;

//	DisplaySpecConfig(specConfigs[0]);

	return 0;
}
