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

private:
	int _fd;
};

#endif // LISTENINGSOCKET_H
