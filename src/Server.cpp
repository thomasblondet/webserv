#include "Server.hpp"

Server::Server(const std::vector<Config> &configs_vector)
{
	// Init all ListeningSocket found in the config file
    for (size_t i = 0; i < configs_vector.size(); i++)
	{
		// Create the ListeningSocket and push it to pollfd
		ListeningSocket s(configs_vector[i].address.c_str(), configs_vector[i].port.c_str());

        struct pollfd listener = { s.get_fd(), POLLIN, 0 };

        _fds.push_back(listener);
    }

    handle_clients();
}

void Server::handle_clients()
{
    std::cout << "Waiting for connection..." << std::endl;

    std::vector<int> listening_sockets;
    for (size_t i = 0; i < _fds.size(); i++)
	{
        listening_sockets.push_back(_fds[i].fd);
		std::cout << "Listening on fd " << _fds[i].fd << "...\n";
	}

    while (1)
	{
        // Pointer to pollfd vector to monitor each socket
        struct pollfd *ppollfds = &_fds[0];
        poll(ppollfds, _fds.size(), -1);

        for (size_t i = 0; i < _fds.size(); i++)
		{
            std::vector<int>::iterator it;
            it = std::find(listening_sockets.begin(), listening_sockets.end(), ppollfds[i].fd);

			// If listening socket => new incoming connection
			// Else read request
            if (it != listening_sockets.end() && ppollfds[i].revents & POLLIN)
				accept_new_connection(_fds[i].fd);
            else if (ppollfds[i].revents & POLLIN)
				handle_request(ppollfds, i);
        }
    }
}

void Server::accept_new_connection(int listening_socket)
{
	struct sockaddr_storage client_addr;
	socklen_t addr_len;
	int fd;

	addr_len = sizeof(client_addr);
	fd = accept(listening_socket, (struct sockaddr *)&client_addr, &addr_len);

	// Push new client to poll
	struct pollfd new_client = {fd, POLLIN, 0};
	_fds.push_back(new_client);
	std::cout << "New connection on fd " << fd << ".\n";
}

// Temp
void print_request(const Request &req)
{
	std::cout << "Method: " << req.get_method() << "\n";
	std::cout << "URI: " << req.get_uri() << "\n";
	std::cout << "HTTP Version: " << req.get_http_version() << "\n";
}

void Server::handle_request(struct pollfd *ppollfds, size_t &i)
{
	char buf[1024];
	ssize_t nrecv;

	std::memset(buf, 0, 1024);
	nrecv = recv(ppollfds[i].fd, buf, 1024, 0);
	if (nrecv <= 0)
	{
		close(ppollfds[i].fd);
		_fds.erase(_fds.begin() + 1);
		i--;
	}
	else
	{
		Request request(buf);
		print_request(request);
	}
}
