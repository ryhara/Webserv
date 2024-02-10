#pragma once

#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
#include "Color.hpp"

void	*ft_memset(void *s, int c, size_t n);
void	log_exit(const char *msg, int line, const char *file, int error);
void	setNonBlockingFd(int fd);
std::string ft_to_string(size_t number);
bool	isHex(const std::string &str);