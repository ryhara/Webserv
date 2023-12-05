#include "Webserv.hpp"

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	parse();
	server();
	std::cout << "webserv" << std::endl;
	return 0;
}
