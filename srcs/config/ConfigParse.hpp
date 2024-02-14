#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "Config.hpp"
#include "ServerConfig.hpp"

class ConfigParse {
	private:
		Config &_config;
		std::vector<std::string> _parseLine;
		std::vector<std::vector<std::string> > _parseLines;
	public:
		ConfigParse();
		ConfigParse(Config &config);
		~ConfigParse(void);

		Config &getConfig(void) const;
		void parse(const std::string &filename);
};