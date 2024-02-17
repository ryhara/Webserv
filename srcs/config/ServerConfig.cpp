#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : port(4242), server_name("default"), max_body_size(100), error_page("./www/error_page/404.html")
{	
}

ServerConfig::~ServerConfig()
{
}

bool isNumber(const std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!std::isdigit(str[i]))
			return false;
	}
	return true;
}

ServerConfig::ServerConfig(std::vector<std::vector<std::string> > &parseLines) : port(4242), server_name("default"), max_body_size(100), error_page("./www/error_page/404.html")
{
	std::vector<std::string> parseLine;
	size_t i = 0;
	for (; i < parseLines.size(); i++)
	{
		parseLine = parseLines[i];
		if (parseLine[0] == "listen")
		{
			if (parseLine.size() != 3)
				log_exit("ServerConfig : invalid config : listen less or more arguments", __LINE__, __FILE__, errno);
			if (!isNumber(parseLine[1]))
				log_exit("ServerConfig : invalid config : port is not numeric", __LINE__, __FILE__, errno);
			if (parseLine[2] != ";")
				log_exit("ServerConfig : invalid config : listen not end with ';'", __LINE__, __FILE__, errno);
			this->port = ft_stoi(parseLine[1]);
			if (this->port < 1 || this->port > 65535)
				log_exit("ServerConfig : invalid config : port is not in range 1-65535", __LINE__, __FILE__, errno);
		}
		else if (parseLine[0] == "server_name")
		{
			if (parseLine.size() != 3)
				log_exit("ServerConfig : invalid config : server_name less or more arguments", __LINE__, __FILE__, errno);
			if (parseLine[2] != ";")
				log_exit("ServerConfig : invalid config : server_name not end with ';'", __LINE__, __FILE__, errno);
			this->server_name = parseLine[1];
		}
		else if (parseLine[0] == "max_body_size")
		{
			if (parseLine.size() != 3)
				log_exit("ServerConfig : invalid config : max_body_size less or more arguments", __LINE__, __FILE__, errno);
			if (!isNumber(parseLine[1]))
				log_exit("ServerConfig : invalid config : max_body_size is not numeric", __LINE__, __FILE__, errno);
			if (parseLine[2] != ";")
				log_exit("ServerConfig : invalid config : max_body_size not end with ';'", __LINE__, __FILE__, errno);
			this->max_body_size = std::stoi(parseLine[1]);
		}
		else if (parseLine[0] == "error_page")
		{
			if (parseLine.size() != 3)
				log_exit("ServerConfig : invalid config : error_page less or more arguments", __LINE__, __FILE__, errno);
			if (parseLine[2] != ";")
				log_exit("ServerConfig : invalid config : error_page not end with ';'", __LINE__, __FILE__, errno);
			this->error_page = parseLine[1];
		}
		else if (parseLine[0] == "location")
		{
			if (parseLine.size() != 3)
				log_exit("ServerConfig : invalid config : location less or more arguments", __LINE__, __FILE__, errno);
			if (parseLine[1][0] != '/' || parseLine[1][parseLine[1].size() - 1] != '/')
				log_exit("ServerConfig : invalid config : location path not start or end with '/'", __LINE__, __FILE__, errno);
			if (parseLine[2] != "{")
				log_exit("ServerConfig : invalid config : location not end with '{'", __LINE__, __FILE__, errno);
			// TODO : newしないとだめ？、フリー忘れそう
			Location *location = new Location(parseLine[1]);
			i = location->addInfo(parseLines, i + 1);
			this->location.insert(std::make_pair(location->get_path(), *location));
		}
		else if (parseLine[0] == "}")
		{
			if (parseLine.size() != 1)
				log_exit("ServerConfig : invalid config : argment before '}'", __LINE__, __FILE__, errno);
			break;
		}
		else
			log_exit("ServerConfig : invalid config : 不適切な要素", __LINE__, __FILE__, errno);
	}
	parseLines.erase(parseLines.begin(), parseLines.begin() + i + 1);
}

std::string ServerConfig::getServerName() const
{
	return this->server_name;
}

int	ServerConfig::getPort() const
{
	return this->port;
}

void ServerConfig::getServerConfig()
{
	std::cout << "+++++++++++++ServerConfig+++++++++++++" << std::endl;
	std::cout << "port: " << this->port << std::endl;
	std::cout << "server_name: " << this->server_name << std::endl;
	std::cout << "max_body_size: " << this->max_body_size << std::endl;
	std::cout << "error_page: " << this->error_page << std::endl;
	std::map<std::string, Location>::iterator it = this->location.begin();
	for (; it != this->location.end(); it++)
	{
		it->second.getLocation();
	}
	std::cout << "++++++++++++++++++++++++++++++++++++++" << std::endl;
}