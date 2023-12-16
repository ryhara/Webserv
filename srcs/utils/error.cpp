#include "utils.hpp"

void log_exit(const char *msg)
{
	std::cerr << "Error: " << msg << std::endl;
	std::exit(EXIT_FAILURE);
}