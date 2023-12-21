#pragma once

#include <string>
#include <iostream>
#include <map>
#include "ServerConfig.hpp"

class Config
{
	private:
		// TODO : 複数のServer configを持てるようにする
		std::map<int, std::vector<ServerConfig> > _servers;
	public :
		Config();
		~Config();
};