#include "Webserv.hpp"

int main(int argc, char **argv)
{
	Server server;
	ConfigParse configParse(server.getConfig());

	if (argc == 1)
	{
		std::cout << "Not set config file path." << std::endl;
		std::cout << "Use default config file: " << DEFAULT_CONFIG_FILE << std::endl;
		// TODO : default.confを読み込む処理
		configParse.parse(DEFAULT_CONFIG_FILE);
	}
	else if (argc != 2)
	{
		std::cout << "Usage: ./webserv [A config_file]" << std::endl;
		return 1;
	}
	// TODO : config_fileを読み込む処理
	if (argc == 2) {
		std::cout << "config file: " << argv[1] << std::endl;
		configParse.parse(argv[1]);
	}
	server.start();
	return 0;
}

__attribute__((destructor))
static void destructor() {
    system("leaks -q webserv");
}
