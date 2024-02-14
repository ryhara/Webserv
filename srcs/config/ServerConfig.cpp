#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {
	
}

ServerConfig::~ServerConfig() {
}

ServerConfig::ServerConfig(std::ifstream &ifs, std::string buf) {
	size_t i = 0;
	while (buf[i] == ' ' || buf[i] == '\t')
		i++;
	// if (buf[i] == '#' || buf[i] == '\0')
	// 	return ;
	if (buf[i] == '{')
	{
		while (std::getline(ifs, buf))
		{
			size_t i = 0;
			while (buf[i] == ' ' || buf[i] == '\t')
				i++;
			if (buf[i] == '\0')
				continue;
			if (buf[i] == '}')
				break;
			if (buf.find("listen ") == i)
			{
				i += 7;
				this->port = std::stoi(&buf[i]);
				if (buf[i + std::to_string(this->port).length()] != ';')
				{
					std::cout << "ServerConfig : invalid config : no ;" << std::endl;
					exit (1);
				}
				i += std::to_string(this->port).length() + 1;
			}
			else if (buf.find("server_name ") == i)
			{
				i += 12;
				this->server_name = buf.substr(i, buf.find(";") - i);
				i += this->server_name.length() + 1;
			}
			else if (buf.find("max_body_size ") == i)
			{
				i += 14;
				this->max_body_size = std::stoi(&buf[i]);
				if (buf[i + std::to_string(this->max_body_size).length()] != ';')
				{
					std::cout << "ServerConfig : invalid config: no ;" << std::endl;
					exit (1);
				}
				i += std::to_string(this->max_body_size).length() + 1;
			}
			else if (buf.find("error_page ") == i)
			{
				i += 11;
				this->error_page = buf.substr(i, buf.find(";") - i);
				i += this->error_page.length() + 1;
			}
			else if (buf.find("location ") == i) //　'location /~ {'　までは一行で書かれている前提({が改行されている場合は考慮していない)
			{
				i += 9;
				while (buf[i] == ' ' || buf[i] == '\t')
					i++;
				if (buf[i] == '/')
				{
					Location location(ifs, &buf[i]);
					this->location.insert(std::make_pair(location.get_path(), location));
				}
				else
				{
					std::cout << "ServerConfig : invalid config : no path after location" << std::endl;
					exit (1);
				}
			}
			else
			{
				std::cout << &buf[i] << std::endl;
				std::cout << "ServerConfig : invalid config : 不適切な要素" << std::endl;
				exit (1);
			}
		}
	}
	else
	{
		std::cout << "ServerConfig : invalid config : no '{' at beginning" << std::endl;
		exit (1);
	}
}

void ServerConfig::getServerConfig()
{
	std::cout << "port: " << this->port << std::endl;
	std::cout << "server_name: " << this->server_name << std::endl;
	std::cout << "max_body_size: " << this->max_body_size << std::endl;
	std::cout << "error_page: " << this->error_page << std::endl;
	for (std::map<std::string, Location>::iterator it = this->location.begin(); it != this->location.end(); it++)
	{
		std::cout << "location: " << it->first << std::endl;
		it->second.getLocation();
	}
}