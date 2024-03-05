#include "utils.hpp"

void ft_sleep(int s)
{
	std::time_t start = std::time(NULL);
	while (std::time(NULL) - start < s)
		;
}

void ft_usleep(int us)
{
	std::time_t start = std::time(NULL);
	while (std::time(NULL) - start < us / 1000000)
		;
}