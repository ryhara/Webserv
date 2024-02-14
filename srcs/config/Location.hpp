#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <map>

class Location
{
private:
	std::string location;
	bool		get_method;
	bool		post_method;
	bool		delete_method;
	std::string alias;
	std::string	index;
	bool		autoindex;
	std::string upload_path;
	std::string cgi_path;
	std::string redir_path;
public:
	Location(/* args */);
	Location(std::ifstream &ifs, std::string buf);
	~Location();
	void getLocation() const;
	std::string get_path() const;
};
