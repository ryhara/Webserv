#include "Webserv.hpp"

int main(int argc, char **argv, char **envp)
{
	(void)argv;
	(void)envp;
	if (argc == 1)
	{
		std::cout << "Use default config file: ./config/default.conf" << std::endl;
		// TODO : default.confを読み込む処理
	}
	// TODO : config_fileを読み込む処理
	if (argc == 2) {
		std::cout << "Use config file: " << argv[1] << std::endl;
	}
	else if (argc != 2)
	{
		std::cerr << "Usage: ./webserv [A config_file]" << std::endl;
		return 1;
	}
	Server server;
	server.start();
	return 0;
}

__attribute__((destructor))
static void destructor() {
    system("leaks -q webserv");
}
