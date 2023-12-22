#include "Webserv.hpp"

int main(int argc, char **argv)
{
	Server server;
	ConfigParse configParse(server.getConfig());

	if (argc == 1)
	{
		std::cout << "Use default config file: ./config/default.conf" << std::endl;
		// TODO : default.confを読み込む処理
		configParse.parse(DEFAULT_CONFIG_FILE);
	}
	else if (argc == 2) {
		std::cout << "Use config file: " << argv[1] << std::endl;
		// TODO : config_fileを読み込む処理
		configParse.parse(argv[1]);
	}
	else if (argc != 2)
	{
		std::cerr << "Usage: ./webserv [A config_file]" << std::endl;
		return 1;
	}
	server.start();
	return 0;
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q webserv");
// }
