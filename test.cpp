#include <iostream>
#include <string>
#include <sstream>

int func(std::stringstream &ss, std::string &line) {
	if (!std::getline(ss, line))
		return (0);
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	return (1);
}

int main(void)
{
	std::stringstream ss("aa\nbb\ncc\n\n");
	std::string line;
	int result;
	while ((result = func(ss, line)))
	{
		std::cout << result << std::endl;
		std::cout << line << std::endl;
	}
	std::cout << result << std::endl;
	std::cout << line << std::endl;
	return (0);
}
