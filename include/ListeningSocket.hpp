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
	int getFd() const;

private:
	int fd;
};

#endif // LISTENINGSOCKET_H
