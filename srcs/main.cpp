#include "Webserv.hpp"

int main(int argc, char **argv, char **envp)
{
	(void)argv;
	(void)envp;
	if (argc == 1)
	{
		std::cout << "Use default path: ./config/default.conf" << std::endl;
		// TODO : default.confを読み込む処理
	}
	else if (argc != 2)
	{
		std::cout << "Usage: ./webserv [A config_file]" << std::endl;
		return 1;
	}
	// TODO : config_fileを読み込む処理
	Server server;
	server.start();
	return 0;
}
