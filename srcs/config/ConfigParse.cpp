#include "ConfigParse.hpp"

ConfigParse::ConfigParse(Config &config) : _config(config) {
}

ConfigParse::~ConfigParse(void) {
}

Config &ConfigParse::getConfig(void) const {
	return (Config &)_config;
}

void ConfigParse::parse(const std::string &filename) {
	std::string word, buf;
	std::cout << "parse config file: " << filename << std::endl;
	std::ifstream ifs(filename);
	if (!ifs)
	{
		std::cout << "cannot open file" << std::endl;
		exit (1);
	}
	while (std::getline(ifs, buf))
	{
		size_t start = 0, end = 0, strlen = buf.length();
		_parseLine.clear();
		while (start < strlen)
		{
			if (buf[start] == ' ' || buf[start] == '\t')
				start++;
			else if (buf[start] == '#' || buf[start] == '\0')
				break;
			else if (buf[start] == '{' || buf[start] == '}' || buf[start] == ';')
			{
				word = buf[start];
				_parseLine.push_back(word);
				start++;
				end = start;
			}
			else
			{
				end = start;
				while (buf[end] != ' ' && buf[end] != '\t' && buf[end] != '#' && buf[end] != '\0'
						&& buf[end] != '{' && buf[end] != '}' && buf[end] != ';')
					end++;
				word = buf.substr(start, end - start);
				_parseLine.push_back(word);
				start = end;
			}
			// if (buf.find("server") == start)
			// {
			// 	// _config.addServer();
			// 	ServerConfig server(ifs, &buf[start+6]);
			// 	this->_config.addServer(server);
			// }
			// else
			// {
			// 	std::cout << "ConfigParse : invalid config : not start with 'server'" << std::endl;
			// 	exit (1);
			// }
		}
		if (!_parseLine.empty())
			_parseLines.push_back(_parseLine);
	}
	ifs.close();
	//validate
	std::string endWord;
	if (_parseLines.empty())
	{
		std::cout << "ConfigParse : invalid config : empty file" << std::endl;
		exit (1);
	}
	if (_parseLines[0].size() != 2 || _parseLines[0][0] != "server" || _parseLines[0][1] != "{")
	{
		std::cout << "ConfigParse : invalid config : not start with 'server {'" << std::endl;
		exit (1);
	}
	_parseLines.erase(_parseLines.begin());
	endWord = _parseLines.back().back();
	if (endWord != "}")
	{
		std::cout << "ConfigParse : invalid config : file not end with '}'" << std::endl;
		exit (1);
	}
	if (_parseLines.back().size() != 1)
	{
		std::cout << " size not 1   ConfigParse : invalid config : argment before '}'" << std::endl;
		exit (1);
	}
	for (size_t i = 0; i < _parseLines.size(); i++)
	{
		endWord = _parseLines[i].back();
		if (endWord != "{" && endWord != ";" && endWord != "}")
		{
			std::cout << "ConfigParse : invalid config : not end with '{' or '}' or ';'" << std::endl;
			exit (1);
		}
		if (endWord == "}" && _parseLines[i].size() != 1)
		{
			std::cout << "ConfigParse : invalid config : argment before '}'" << std::endl;
			exit (1);
		}
	}
	//parse
	ServerConfig server(_parseLines);
	this->_config.addServer(server);
}