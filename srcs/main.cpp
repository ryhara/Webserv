#include "ConfigParse.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include "ServerConfig.hpp"
#include "Webserv.hpp"

// TODO : 全クラスにおいてデストラクタやerror時のfreeやclear処理を実装する

void printServerInfo(std::map<int, std::vector<ServerConfig> > &servers)
{
	std::map<int, std::vector<ServerConfig> >::iterator it = servers.begin();
	for (; it != servers.end(); it++)
	{
		std::cout << "port : " << it->first << std::endl;
		for (size_t i = 0; i < it->second.size(); i++)
		{
			it->second[i].printServerConfig();
			std::cout << "server_name: " << it->second[i].getServerName() << std::endl;
		}
	}
}

int main(int argc, char **argv)
{
	Server server;
	ConfigParse configParse(server.getConfig());

	if (argc == 1) {
		configParse.parse(DEFAULT_CONFIG_FILE);
	}
	else if (argc == 2) {
		configParse.parse(argv[1]);
	}
	else if (argc != 2) {
		std::cerr << "Usage: ./webserv [A config_file]" << std::endl;
		return 1;
	}
	server.setServers(configParse.getConfig().getServers());
	#if DEBUG
		printServerInfo(server.getConfig().getServers());
	#endif
	server.start();
	return 0;
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q webserv");
// }
