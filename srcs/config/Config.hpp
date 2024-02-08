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
		DISALLOW_COPY_AND_ASSIGN(Config);
	public :
		Config();
		~Config();
};