#ifndef QUERYPARSE_HPP
#define QUERYPARSE_HPP

#include <iostream>
#include <vector>

class QueryParse {
	public:
		std::vector<std::string> split(std::string uri);
};

#endif