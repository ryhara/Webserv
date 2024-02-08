#pragma once

#include <string>
#include <iostream>
#include "Config.hpp"

class ConfigParse {
	private:
		Config &_config;
		DISALLOW_COPY_AND_ASSIGN(ConfigParse);
	public:
		ConfigParse(Config &config);
		~ConfigParse(void);

		Config &getConfig(void) const;
		void parse(const std::string &filename);
};