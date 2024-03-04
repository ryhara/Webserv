#include "utils.hpp"

void setNonBlockingAndCloseOnExec(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
		log_exit("fcntl/O_NONBLOCK", __LINE__, __FILE__, errno);
	}
	if (fcntl(fd, 2, FD_CLOEXEC) < 0) {
		log_exit("fcntl/FD_CLOEXEC", __LINE__, __FILE__, errno);
	}
}

void setNonBlockingFd(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
		log_exit("fcntl/O_NONBLOCK", __LINE__, __FILE__, errno);
	}
}

void setCloseOnExec(int fd)
{
	if (fcntl(fd, 2, FD_CLOEXEC) < 0) {
		log_exit("fcntl/FD_CLOEXEC", __LINE__, __FILE__, errno);
	}
}
