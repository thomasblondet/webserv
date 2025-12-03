#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(const char *address, const char *port)
: _address(std::string(address)), _port(std::string(port))
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
	
	_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (_fd == -1)
		throw std::runtime_error("socket");

	status = setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (status == -1)
		throw std::runtime_error("setsockopt");

	status = bind(_fd, res->ai_addr, res->ai_addrlen);
	if (status == -1)
		throw std::runtime_error("bind");

	freeaddrinfo(res);

	status = listen(_fd, 10);
	if (status == -1)
		throw std::runtime_error("listen");
}

ListeningSocket::~ListeningSocket()
{
}

int ListeningSocket::get_fd() const
{
	return _fd;
}

const std::string ListeningSocket::get_address() const
{
	return _address;
}

const std::string ListeningSocket::get_port() const
{
	return _port;
}