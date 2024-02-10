#include "utils.hpp"

void log_exit(const char *msg, int line, const char *file, int error)
{
	std::cerr << RED << "Error: " << msg << END
		<< " (line : " << UNDERLINE << line << END <<
		", file : "<< UNDERLINE << file << END << ")" << std::endl;
	std::cerr << "-- " << strerror(error) << " --" << std::endl;
	std::exit(EXIT_FAILURE);
}

void log_print(const char *msg, int line, const char *file, int error)
{
	std::cerr << RED << "Error: " << msg << END
		<< " (line : " << UNDERLINE << line << END <<
		", file : "<< UNDERLINE << file << END << ")" << std::endl;
	std::cerr << "-- " << strerror(error) << " --" << std::endl;
}