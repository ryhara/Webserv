#include "utils.hpp"

bool isHex(const std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!std::isxdigit(str[i]))
			return false;
	}
	return true;
}