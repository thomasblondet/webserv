#ifndef LISTENINGSOCKET_H
#define LISTENINGSOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#include <iostream>
#include <exception>
#include <cstring>

class ListeningSocket
{
public:
	ListeningSocket(const char *address, const char *port);
	~ListeningSocket();
	int get_fd() const;
	const std::string get_address() const;
	const std::string get_port() const;
private:
	int _fd;
	const std::string _address;
	const std::string _port;
};

#endif // LISTENINGSOCKET_H
