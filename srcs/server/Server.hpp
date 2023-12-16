#pragma once

#include <iostream>
#include <string>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>

#include "Color.hpp"
#include "utils.hpp"

# define DEFAULT_SERVER_PORT	4242
# define MAX_CLIENTS			10

class Server
{
	public:
		Server(void);
		~Server(void);

		void start(void);
};
