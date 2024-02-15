#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "ServerConfig.hpp"
#include "Webserv.hpp"

class Config
{
	private:
		// TODO : 複数のServer configを持てるようにする
		std::map<int, std::vector<ServerConfig> > _servers;
		// DISALLOW_COPY_AND_ASSIGN(Config);
	public :
		Config();
		~Config();
		void addServer(ServerConfig &server);
		// ServerConfig &getServer() const;
		// std::map<std::string, ServerConfig> &getServers() { return _servers; }
		std::map<int, std::vector<ServerConfig> > &getServers() { return _servers; }
};