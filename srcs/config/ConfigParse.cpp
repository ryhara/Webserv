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
	std::ifstream ifs(filename);
	if (!ifs)
	{
		std::cout << "cannot open file" << std::endl;
		exit (1);
	}
	while (std::getline(ifs, buf))
	{
		size_t i = 0;
		while (buf[i] == ' ' || buf[i] == '\t')
			i++;
		if (buf[i] == '#' || buf[i] == '\0')
			continue;
		if (buf.find("server") == i)
		{
			// _config.addServer();
			ServerConfig server(ifs, &buf[i+6]);
			this->_config.addServer(server);
		}
		else
		{
			std::cout << "ConfigParse : invalid config : not start with 'server'" << std::endl;
			exit (1);
		}
	}
}