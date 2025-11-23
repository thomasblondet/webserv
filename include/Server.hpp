#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <vector>
#include <cstring> // std::memset
#include <algorithm> // std::find

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/poll.h>
#include <unistd.h>

#include "Parser.hpp"
#include "ListeningSocket.hpp"
#include "Request.hpp"

class Server
{
public:
    Server(const std::vector<Config> &configs_vector);

private:
	std::vector<struct pollfd> _fds;

	void handle_clients();
	void accept_new_connection(int listening_socket);
	void handle_request(struct pollfd *ppollfds, size_t &i);
};

#endif // SERVER_H
