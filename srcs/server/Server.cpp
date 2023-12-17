#include "Server.hpp"

Server::Server(void)
{
}

Server::~Server(void)
{
}

// TODO : split into smaller functions
void Server::start(void)
{
	std::cout << "Server::start" << std::endl;
	struct sockaddr_in server_addr, client_addr;
	int server_fd, client_fd, pid;
	socklen_t client_addr_len;

	// Create socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		std::cerr << "Error: socket" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	// TODO : setsockopt ?
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, NULL, 0);

	// Init server_addr
	ft_memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(DEFAULT_SERVER_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// Bind
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		close(server_fd);
		log_exit("bind");
	}
	// Listen
	if (listen(server_fd, MAX_CLIENTS) < 0)
	{
		close(server_fd);
		log_exit("listen");
	}
	for (;;)
	{
		// Accept
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
		if (client_fd < 0)
		{
			close(server_fd);
			log_exit("accept");
		}
		// Fork
		// TODO : signal handle
		// TODO : forkはCGIの時だけにする　pollを使う
		pid = fork();
		if (pid < 0)
		{
			close(server_fd);
			close(client_fd);
			log_exit("fork");
		}
		else if (pid == 0)
		{
			// Child process
			close(server_fd);
			// TODO : buffer size調整、第3引数の調べる(MSG_DONTWAIT : ノンブロッキングモードなので使えそう)
			char buffer[BUFFER_SIZE];
			ssize_t n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
			if (n < 0)
			{
				close(client_fd);
				log_exit("recv");
			}
			buffer[n] = '\0';
			std::cout << "Received: " << buffer << std::endl;

			close(client_fd);
			std::cout << "Client connected" << std::endl;
			exit(EXIT_SUCCESS);
		}
		else
		{
			int status;
			// Parent process
			close(client_fd);
			if (waitpid(pid, &status, 0) < 0)
			{
				close(server_fd);
				log_exit("waitpid");
			}
		}

	}
}