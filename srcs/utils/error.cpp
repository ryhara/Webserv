#include "utils.hpp"
#include "Color.hpp"

void log_exit(const char *msg, int line, const char *file)
{
	std::cerr << RED << "Error: " << msg << END
		<< " (line : " << UNDERLINE << line << END <<
		", file : "<< UNDERLINE << file << END << ")" << std::endl;
	std::perror(msg);
	std::exit(EXIT_FAILURE);
}