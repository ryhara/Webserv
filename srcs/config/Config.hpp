#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "ServerConfig.hpp"

class Config
{
	private:
		// TODO : 複数のServer configを持てるようにする
		// std::map<int, std::vector<ServerConfig> > _servers;
		std::map<std::string, ServerConfig> _servers;
	public :
		Config();
		~Config();
		void addServer(ServerConfig &server);
		// ServerConfig &getServer() const;
		std::map<std::string, ServerConfig> &getServers() { return _servers; }
};