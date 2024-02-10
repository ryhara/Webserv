#include "utils.hpp"

std::string ft_to_string(size_t number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}