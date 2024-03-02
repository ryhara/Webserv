#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <cstdlib>

class Location
{
	private:
		std::string location;
		bool		get_method;
		bool		post_method;
		bool		delete_method;
		bool		autoindex;
		std::string alias;
		std::string	index;
		std::string upload_path;
		std::string redir_path;
		std::vector<std::string> cgi_extension;

	public:
		Location(/* args */);
		Location(std::string location);
		~Location();

		size_t		addInfo(std::vector<std::vector<std::string> > &parseLines, size_t startIndex);
		void	getLocation() const;
		std::string get_path() const;
};
