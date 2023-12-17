#include "utils.hpp"

void log_exit(const char *msg, int line, const char *file)
{
	std::cerr << "Error: " << msg << std::endl;
	std::cerr << "line : " << line << ", file : " << file << std::endl;
	std::exit(EXIT_FAILURE);
}