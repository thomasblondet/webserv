#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(const char *address, const char *port)
{
	struct addrinfo hints;
	struct addrinfo *res;
	int status;
	int yes = 1;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(address, port, &hints, &res);
	if (status != 0)
		throw std::runtime_error("getaddrinfo");
	
	fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (fd == -1)
		throw std::runtime_error("socket");

	status = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (status == -1)
		throw std::runtime_error("setsockopt");

	status = bind(fd, res->ai_addr, res->ai_addrlen);
	if (status == -1)
		throw std::runtime_error("bind");

	freeaddrinfo(res);

	status = listen(fd, 10);
	if (status == -1)
		throw std::runtime_error("listen");
}

ListeningSocket::~ListeningSocket()
{
}

int ListeningSocket::getFd() const
{
	return fd;
}
