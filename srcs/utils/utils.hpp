#pragma once

#include <iostream>
#include <string>
#include <fcntl.h>

void	*ft_memset(void *s, int c, size_t n);
void	log_exit(const char *msg, int line, const char *file, int error);
void	setNonBlockingFd(int fd);