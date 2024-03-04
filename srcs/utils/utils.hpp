#pragma once

#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <fcntl.h>
#include "Color.hpp"

void	*ft_memset(void *s, int c, size_t n);
void	log_exit(const char *msg, int line, const char *file, int error);
void	log_print(const char *msg, int line, const char *file, int error);
void	setNonBlockingAndCloseOnExec(int fd);
void	setCloseOnExec(int fd);
void	setNonBlockingFd(int fd);
std::string	ft_to_string(size_t number);
int		ft_stoi(const std::string &str);
bool	isHex(const std::string &str);
