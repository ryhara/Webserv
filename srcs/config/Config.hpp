#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "ServerConfig.hpp"
#include "Webserv.hpp"

// 複数のServerConfigを保持
class Config
{
	private:
		std::map<int, std::vector<ServerConfig> > _servers;
		DISALLOW_COPY_AND_ASSIGN(Config);
	public :
		Config();
		~Config();
		void addServer(ServerConfig &server);
		std::map<int, std::vector<ServerConfig> > &getServers() { return _servers; }
};