#pragma once

#include <iostream>
#include <sstream>

class SurpressOutput {
	private:
		std::stringstream buffer;
		std::streambuf* prevcoutbuf;
	public:
		SurpressOutput(void) {
			prevcoutbuf = std::cout.rdbuf();
			std::cout.rdbuf(buffer.rdbuf());
		}
		~SurpressOutput(void) {
			std::cout.rdbuf(prevcoutbuf);
		}
};