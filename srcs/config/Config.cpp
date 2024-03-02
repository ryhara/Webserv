#include "Config.hpp"

Config::Config() {
}

Config::~Config() {
	_servers.clear();
}

void Config::addServer(ServerConfig &server)
{
	std::map<int, std::vector<ServerConfig> >::iterator it = this->_servers.begin();
	for (; it != this->_servers.end(); it++)
	{
		for (size_t i = 0; i < it->second.size(); i++)
		{
			if (it->second[i].getServerName() == server.getServerName())
				log_exit("Config : invalid config : server name is duplicated", __LINE__, __FILE__, errno);
		}
	}
	it = this->_servers.find(server.getPort());
	if (it != this->_servers.end())
	{
		it->second.push_back(server);
		return ;
	}
	else
	{
		std::vector<ServerConfig> servers;
		servers.push_back(server);
		this->_servers.insert(std::make_pair(server.getPort(), servers));
	}
}
