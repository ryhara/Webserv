#include "utils.hpp"
#include "Color.hpp"

void log_exit(const char *msg, int line, const char *file, int error)
{
	std::cerr << RED << "Error: " << msg << END
		<< " (line : " << UNDERLINE << line << END <<
		", file : "<< UNDERLINE << file << END << ")" << std::endl;
	std::cout << "-- " << strerror(error) << " --" << std::endl;
	std::exit(EXIT_FAILURE);
}