#include "Location.hpp"

Location::Location(/* args */) : get_method(true), post_method(false), delete_method(false), autoindex(false)
{
}

Location::~Location()
{
}

Location::Location(std::ifstream &ifs, std::string buf) : get_method(true), post_method(false), delete_method(false), autoindex(false)
{
	size_t i = 0, semicolon = 0;
	if (buf[1] == ' ' || buf[1] == '\t')
	{
		this->location = "/";
		i = 2;
		while (buf[i] == ' ' || buf[i] == '\t')
			i++;
		if (buf[i] != '{')
		{
			std::cout << "Location: invalid config : no '{' after server" << std::endl;
			exit (1);
		}
		i++;
		while (buf[i] == ' ' || buf[i] == '\t')
			i++;
		if (buf[i] != '\0')
		{
			std::cout << "Location: invalid config : not \\0" << std::endl;
			exit (1);
		}
		while(std::getline(ifs, buf))
		{
			i = 0;
			// std::cout << "+++++++++++++++++" << std::endl;
			// std::cout << buf << std::endl;
			// std::cout << "+++++++++++++++++" << std::endl;

			while (buf[i] != '\0')
			{
				while (buf[i] == ' ' || buf[i] == '\t')
					i++;
				if (buf[i] == '\0')
					break;
				if (buf[i] == '}')
					return;
				if (buf.find("alias ") == i)
				{
					i += 6;
					if ((semicolon = buf.find(";")) == std::string::npos)
					{
						std::cout << "Location: invalid config : no ;" << std::endl;
						exit (1);
					}
					this->alias = buf.substr(i, semicolon - i);
					i = semicolon + 1;
				}
				else if (buf.find("index ") == i)
				{
					i += 6;
					if ((semicolon = buf.find(";")) == std::string::npos)
					{
						std::cout << "Location: invalid config : no ;" << std::endl;
						exit (1);
					}
					this->index = buf.substr(i, semicolon - i);
					i = semicolon + 1;
				}
				else if (buf.find("autoindex ") == i)
				{
					i += 10;
					if ((semicolon = buf.find(";")) == std::string::npos)
					{
						std::cout << "Location: invalid config : no ;" << std::endl;
						exit (1);
					}
					while (buf[i] != ';')
					{
						if (buf[i] == ' ' || buf[i] == '\t')
							i++;
						else if (buf[i] == 'o' && buf[i + 1] == 'n')
						{
							this->autoindex = true;
							i += 2;
							while (buf[i] != ';')
							{
								if (buf[i] != ' ' && buf[i] != '\t')
								{
									std::cout << "Location: invalid config : autoindex 不要なアルファベット" << std::endl;
									exit (1);
								}
								i++;
							}
							i++;
							break;
						}
						else if (buf[i] == 'o' && buf[i + 1] == 'f' && buf[i + 2] == 'f')
						{
							this->autoindex = false;
							i += 3;
							while (buf[i] != ';')
							{
								if (buf[i] != ' ' && buf[i] != '\t')
								{
									std::cout << "Location: invalid config : autoindex 不要なアルファベット" << std::endl;
									exit (1);
								}
								i++;
							}
							i++;
							break;
						}
						else 
						{
							std::cout << "Location: invalid config : autoindex on, off 以外の入力" << std::endl;
							exit (1);
						}
					}
					
				}
				else if (buf.find("upload_path ") == i)
				{
					i += 12;
					if ((semicolon = buf.find(";")) == std::string::npos)
					{
						std::cout << "Location: invalid config : no ;" << std::endl;
						exit (1);
					}
					this->upload_path = buf.substr(i, semicolon - i);
					i = semicolon + 1;
				}
				else if (buf.find("cgi_path ") == i)
				{
					i += 9;
					if ((semicolon = buf.find(";")) == std::string::npos)
					{
						std::cout << "Location: invalid config : no ;" << std::endl;
						exit (1);
					}
					this->cgi_path = buf.substr(i, semicolon - i);
					i = semicolon + 1;
				}
				else if (buf.find("redir_path ") == i)
				{
					i += 11;
					if ((semicolon = buf.find(";")) == std::string::npos)
					{
						std::cout << "Location: invalid config : no ;" << std::endl;
						exit (1);
					}
					this->redir_path = buf.substr(i, semicolon - i);
					i = semicolon + 1;
				}
				else if (buf.find("method ") == i)
				{
					i += 7;
					if ((semicolon = buf.find(";")) == std::string::npos)
					{
						std::cout << "Location: invalid config : no ;" << std::endl;
						exit (1);
					}
					std::string method = buf.substr(i, semicolon - i);
					if (method.find("POST") != std::string::npos)
						this->post_method = true;
					if (method.find("DELETE") != std::string::npos)
						this->delete_method = true;
					i = semicolon + 1;
				}
				else
				{
					std::cout << &buf[i] << std::endl;
					std::cout << "Location: invalid config : 不適切な要素" << std::endl;
					exit (1);
				}
			}
		}
		std::cout << "Location: invalid config : no '}' at end" << std::endl;
		exit (1);
	}
}

std::string Location::get_path() const
{
	return this->location;
}

void Location::getLocation() const
{
	std::cout << "location: " << this->location << std::endl;
	std::cout << "alias: " << this->alias << std::endl;
	std::cout << "index: " << this->index << std::endl;
	std::cout << "autoindex: " << this->autoindex << std::endl;
	std::cout << "upload_path: " << this->upload_path << std::endl;
	std::cout << "cgi_path: " << this->cgi_path << std::endl;
	std::cout << "redir_path: " << this->redir_path << std::endl;
	std::cout << "get_method: " << this->get_method << std::endl;
	std::cout << "post_method: " << this->post_method << std::endl;
	std::cout << "delete_method: " << this->delete_method << std::endl;
}