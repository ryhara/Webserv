#include "Webserv.hpp"

int main(int argc, char **argv, char **envp)
{
	(void)argv;
	(void)envp;
	if (argc != 2)
	{
		std::cout << "Usage: ./webserv [config_file]" << std::endl;
		return 1;
	}
	parse();
	server();
	std::cout << "webserv" << std::endl;
	return 0;
}
