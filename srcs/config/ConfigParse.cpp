#include "ConfigParse.hpp"

ConfigParse::ConfigParse(Config &config) : _config(config) {
}

ConfigParse::~ConfigParse(void) {
}

Config &ConfigParse::getConfig(void) const {
	return (Config &)_config;
}

void ConfigParse::parse(const std::string &filename) {
	std::cout << "parse config file: " << filename << std::endl;
}