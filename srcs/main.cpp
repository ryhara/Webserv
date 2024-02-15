#include "ConfigParse.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include "ServerConfig.hpp"
#include "Webserv.hpp"

// TODO : 全クラスにおいてデストラクタやerror時のfreeやclear処理を実装する

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	Server server;
	ConfigParse configParse(server.getConfig());
	configParse.parse(DEFAULT_CONFIG_FILE);
	std::map<int, std::vector<ServerConfig> > servers = configParse.getConfig().getServers();
	std::map<int, std::vector<ServerConfig> >::iterator it = servers.begin();
	for (; it != servers.end(); it++)
	{
		std::cout << "port : " << it->first << std::endl;
		for (size_t i = 0; i < it->second.size(); i++)
		{
			// it->second[i].getServerConfig();
			std::cout << "server_name: " << it->second[i].getServerName() << std::endl;
		}
	}
	return 0;
}

// int main(int argc, char **argv)
// {
// 	Server server;
// 	ConfigParse configParse(server.getConfig());

// 	if (argc == 1)
// 	{
// 		std::cout << "Use default config file: ./config/default.conf" << std::endl;
// 		// TODO : default.confを読み込む処理
		// configParse.parse(DEFAULT_CONFIG_FILE);
// 	}
// 	else if (argc == 2) {
// 		std::cout << "Use config file: " << argv[1] << std::endl;
// 		// TODO : config_fileを読み込む処理
// 		configParse.parse(argv[1]);
// 	}
// 	else if (argc != 2)
// 	{
// 		std::cerr << "Usage: ./webserv [A config_file]" << std::endl;
// 		return 1;
// 	}
// 	server.start();
// 	return 0;
// }

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q webserv");
// }
