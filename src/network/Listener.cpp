#include "Listener.hpp"
#include <iostream>

Listener::Listener(EventManager *m, ServerConfig *c) : manager(m), config(c)
{
	struct addrinfo hints;
	struct addrinfo *res;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    int r = getaddrinfo(config->getHost().c_str(), config->getPort().c_str(), &hints, &res);
    if (r != 0)
    {
        std::cerr << "getaddrinfo() failed: " << gai_strerror(r) << std::endl;
        throw std::runtime_error("");
    }
    fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd == -1)
    {
        freeaddrinfo(res);
        std::cerr << "socket() failed" << std::endl;
        throw std::runtime_error("");
    }
    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        freeaddrinfo(res);
        std::cerr << "setsockopt() failed" << std::endl;
        throw std::runtime_error("");
    }
    if (bind(fd, res->ai_addr, res->ai_addrlen) == -1)
    {
        freeaddrinfo(res);
        std::cerr << "bind() failed" << std::endl;
        throw std::runtime_error("");
    }
    freeaddrinfo(res);

    if (listen(fd, 10) == -1)
    {
        std::cerr << "listen() failed" << std::endl;
        throw std::runtime_error("");
    }
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        std::cerr << "fcntl F_GETFL" << std::endl;
        throw std::runtime_error("");
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        std::cerr << "fcntl F_SETFL" << std::endl;
        throw std::runtime_error("");
    }
    std::cout << "* Listening on " << config->getHost() << ":"
        << config->getPort() << "..." << std::endl << std::endl;
}

Listener::~Listener() {}

int Listener::getFD() const
{
	return fd;
}

const std::string Listener::getType() const
{
	return "Listener";
}

void Listener::onRead()
{
	struct sockaddr_storage clientAddress;
    socklen_t addressLen = sizeof(clientAddress);
    int newFD;
    
    newFD = accept(fd, (struct sockaddr *)&clientAddress, &addressLen);
    if (newFD == -1)
    {
        std::cerr << "accept() failed" << std::endl;
        throw std::runtime_error("");
    }

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    int res = getnameinfo(
        (struct sockaddr *)&clientAddress,
        addressLen,
        host,
        sizeof(host),
        service,
        sizeof(service),
        NI_NUMERICHOST | NI_NUMERICSERV
    );

    if (res != 0)
        std::cerr << "getnameinfo() failed: " << gai_strerror(res) << std::endl;

    Client *client = new Client(newFD, manager, config, host, service);
    manager->add(client, POLLIN | POLLOUT);
}

void Listener::onWrite() {}

bool Listener::isClosed() const
{
    return false;
}