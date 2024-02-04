#include "utils.hpp"


// TODO : 使い方については、さらに調べる。
void setNonBlockingFd(int fd)
{
    if (fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
    {
        log_exit("fcntl", __LINE__, __FILE__, errno);
    }
	int flag = fcntl(fd, F_GETFL);
	if (flag < 0) {
		log_exit("fcntl", __LINE__, __FILE__, errno);
	} else {
		if (fcntl(fd, F_SETFL, flag | O_NONBLOCK) < 0) {
			log_exit("fcntl/F_SETFL", __LINE__, __FILE__, errno);
		}
		if (fcntl(fd, F_SETFD, FD_CLOEXEC) < 0) {
			log_exit("fcntl/F_SETFD", __LINE__, __FILE__, errno);
		}
	}
}