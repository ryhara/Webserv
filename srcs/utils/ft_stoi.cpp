#include "utils.hpp"

int ft_stoi(const std::string &str)
{
	std::stringstream ss(str);
	int result;
	ss >> result;
	return (result);
}
