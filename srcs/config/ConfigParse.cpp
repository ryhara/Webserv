#include "ConfigParse.hpp"

ConfigParse::ConfigParse() : _config(*new Config()) {
}

ConfigParse::ConfigParse(Config &config) : _config(config) {
}

ConfigParse::~ConfigParse(void) {
}

Config &ConfigParse::getConfig(void) const {
	return (Config &)_config;
}

void ConfigParse::parse(const std::string &filename) {
	std::string word, buf;
	std::cout << "Config file: " << filename << std::endl << std::endl;
	std::ifstream ifs(filename);
	if (!ifs)
		log_exit("ConfigParse : invalid config : cannot open file", __LINE__, __FILE__, errno);
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
		}
		if (!_parseLine.empty())
			_parseLines.push_back(_parseLine);
	}
	ifs.close();
	//validate
	std::string endWord;
	if (_parseLines.empty())
		log_exit("ConfigParse : invalid config : empty file", __LINE__, __FILE__, errno);
	endWord = _parseLines.back().back();
	if (endWord != "}")
		log_exit("ConfigParse : invalid config : file not end with '}'", __LINE__, __FILE__, errno);
	if (_parseLines.back().size() != 1)
		log_exit("ConfigParse : invalid config : argment before '}'", __LINE__, __FILE__, errno);
	for (size_t i = 0; i < _parseLines.size(); i++)
	{
		endWord = _parseLines[i].back();
		if (endWord != "{" && endWord != ";" && endWord != "}")
			log_exit("ConfigParse : invalid config : not end with '{' or '}' or ';'", __LINE__, __FILE__, errno);
		if (endWord == "}" && _parseLines[i].size() != 1)
			log_exit("ConfigParse : invalid config : argment before '}'", __LINE__, __FILE__, errno);
	}
	//parse
	while (!_parseLines.empty())
	{
		if (_parseLines[0].size() != 2 || _parseLines[0][0] != "server" || _parseLines[0][1] != "{")
			log_exit("ConfigParse : invalid config : not start with 'server {'", __LINE__, __FILE__, errno);
		_parseLines.erase(_parseLines.begin());
		ServerConfig server(_parseLines);
		this->_config.addServer(server);
	}
}