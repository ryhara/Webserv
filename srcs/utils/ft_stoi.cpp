#include "utils.hpp"

std::string ft_stoi(size_t number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}