#pragma once

#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>

void	*ft_memset(void *s, int c, size_t n);
void	log_exit(const char *msg, int line, const char *file, int error);
void	setNonBlockingFd(int fd);
std::string ft_stoi(size_t number);