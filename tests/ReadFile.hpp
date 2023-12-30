#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#define CRLF "\r\n"

class ReadFile {
	private:
		std::string _body;
	public:
		ReadFile(std::string path) {
			std::ifstream ifs(path);
			if (!ifs) {
				std::cerr << "Error: file not opened." << std::endl;
				exit(1);
			}
			while (ifs) {
				std::string line;
				getline(ifs, line);
				_body += line;
				_body += CRLF;
			}
			ifs.close();
		};
		~ReadFile(void) {};
		const std::string &getBody() const {
			return _body;
		};
};