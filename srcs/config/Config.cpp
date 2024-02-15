#include "Config.hpp"

// TODO : config 初期化, configのデータの持たせ方は調べながら考える
Config::Config() {
}

Config::~Config() {
}

// void Config::addServer(ServerConfig &server)
// {
// 	this->_servers[0].push_back(server);
// }

void Config::addServer(ServerConfig &server)
{
	this->_servers.insert(std::make_pair(server.getServerName(), server));
}

// ServerConfig &Config::getServer() const
// {
// 	return (ServerConfig &)_servers[0][0];
// }