#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "Config.hpp"
#include "ServerConfig.hpp"

class ConfigParse {
	private:
		Config &_config;
		std::string buf;
	public:
		ConfigParse();
		ConfigParse(Config &config);
		~ConfigParse(void);

		Config &getConfig(void) const;
		void parse(const std::string &filename);
};