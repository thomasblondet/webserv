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

#include "Parser.h"

typedef struct Request {
	// Status Line
	std::string method;
	std::string uri;
	std::string version;
	// Header
	std::map<std::string, std::string> headers;
	// Body (POST)
	std::string body;
} Request;

class Server {
public:
    Server(const std::vector<Config>& v);
	void print(std::vector<Config> v) const; // tmp
private:
	std::vector<Config> m_cfgs;
	std::vector<struct pollfd> m_fds;
	int init_listener(Config cfg);
	void accept_connection();
	Request parse_request(const std::string& buf);
};

#endif // SERVER_H
