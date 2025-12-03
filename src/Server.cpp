#include "Server.hpp"

Server::Server(const std::vector<Config> &configs)
{
	// For every listeners in the config file
	// Create a listening socket
	// And add it to monitor
	// And add it to the listeners array
    for (size_t i = 0; i < configs.size(); i++)
	{
		// Create the ListeningSocket and push it to pollfd
		ListeningSocket sock(configs[i].address.c_str(), configs[i].port.c_str());

        struct pollfd fd = { sock.get_fd(), POLLIN, 0 };
		_monitor.push_back(fd);
        _listeners.push_back(fd.fd);

		std::cout << "Listening on " << sock.get_address() << ":" << sock.get_port() << "\n";
    }

	std::cout << "Waiting for connection...\n\n";

	while (true)
	{
		int poll_count = poll(&_monitor[0], _monitor.size(), -1);
		if (poll_count == -1)
			std::runtime_error("poll");
		
		process_events();
	}
    
}

void Server::process_events()
{
	// Process all connections
	for (size_t i = 0; i < _monitor.size(); i++)
	{
		// Check if an event occured
		if (_monitor[i].revents & (POLLIN | POLLHUP))
		{
			// If the fd triggered is the listener fd
			// It's a new connection
			std::vector<int>::const_iterator it;
			it = std::find(_listeners.begin(), _listeners.end(), _monitor[i].fd);
			if (it != _listeners.end() && _monitor[i].revents & POLLIN)
			{
				std::cout << "New Connection\n";
				handle_new_connection(_listeners[i]);
			}
			// Otherwise this is data to be read
			else
			{
				handle_client_data(i);
			}
		}
	}
}

void Server::handle_new_connection(int listening_socket)
{
	struct sockaddr_storage client_addr;
	socklen_t addr_len;
	int fd;

	addr_len = sizeof(client_addr);
	fd = accept(listening_socket, (struct sockaddr *)&client_addr, &addr_len);
	if (fd == -1)
		throw std::runtime_error("accept");
	
	// Push new client to monitor
	struct pollfd new_client = {fd, POLLIN, 0};
	_monitor.push_back(new_client);
}

// Temp
void print_request(const Request &req)
{
	std::cout << "Method: " << req.get_method() << "\n";
	std::cout << "URI: " << req.get_uri() << "\n";
	std::cout << "HTTP Version: " << req.get_http_version() << "\n";
}

void Server::handle_client_data(size_t &pos)
{
	char buf[1024];
	ssize_t n;

	std::memset(buf, 0, 1024);
	n = recv(_monitor[pos].fd, buf, 1024, 0);
	if (n <= 0)
	{
		close(_monitor[pos].fd);
		_monitor.erase(_monitor.begin() + 1);
		pos--;
	}
	else
	{
		Request request(buf);
		//print_request(request);
		Response response(request);
		send(_monitor[pos].fd, response.get_response().c_str(), response.get_response().size(), 0);
	}
}
