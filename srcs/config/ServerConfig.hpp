#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include "Location.hpp"

class ServerConfig
{
	private:
		// TODO : server１つ分の情報を保持する
		int			port;
		std::string server_name;
		int			max_body_size;
		std::string error_page;
		std::map<std::string, Location> location;
	public:
		ServerConfig();
		~ServerConfig();
		ServerConfig(std::vector<std::vector<std::string> > &parseLines);
		// TODO : getter
		std::string getServerName() const;
		void getServerConfig();
		// TODO : setter
};