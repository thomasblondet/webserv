#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/poll.h>
#include <unistd.h>

#include "Parser.hpp"
#include "ListeningSocket.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Server
{
public:
    Server(const std::vector<Config> &configs);

private:
	std::vector<struct pollfd> _monitor;
	std::vector<int> _listeners;

	void process_events();
	void handle_new_connection(int listening_fd);
	void handle_client_data(size_t &pos);
};

#endif // SERVER_H
