#include "Client.hpp"
#include "HTTPRequest.hpp"
#include <cstddef>
#include <cstdio>
#include <iostream>

Client::Client(int fd, EventManager *m, ServerConfig *c)
    : fd(fd), manager(m), config(c), closed(false), writeOffset(0), handler(NULL)
{
	(void)manager;
	parser = new HTTPParser;
	std::cout << "* Connection (" << fd << ")" << std::endl;
}


Client::Client(int fd, EventManager *m, ServerConfig *c, std::string ip, std::string port):
	fd(fd),
	manager(m),
	config(c),
	closed(false),
	writeOffset(0),
	handler(NULL),
	ipStr(ip),
	portStr(port)
{
	parser = new HTTPParser;
	std::cout << "\n* Connection [" << ipStr << ":" << portStr << "] (" << fd << ") new" << std::endl;
}

Client::~Client()
{
	if (ipStr.empty() && portStr.empty())
    	std::cout << "* Connection closed (" << fd << ")" << std::endl << std::endl;
	else
		std::cout << "* Connection [" << ipStr << ":" << portStr << "] (" << fd << ") closed" << std::endl;
	if (fd >= 0)
		close(fd);
    delete parser;
	delete handler;
}

int Client::getFD() const
{
	return fd;
}

const std::string Client::getType() const
{
	return "Client";
}

void Client::onRead()
{
	char buf[1027];
	std::memset(buf, 0, 1027);

	std::size_t n = recv(fd, buf, sizeof(buf), 0);
	if (n <= 0)
	{
		std::cerr << "Unable to read from [" << this->getIP() << ":" << this->getPort() << "]\n";
		closed = true;
		return;
	}

	if (parser->parse(buf, n))
	{
		HTTPRequest request = parser->getRequest();
		request.setClientIP(this->getIP());
		request.setclientPort(this->getPort());
		loggingMsgIncomingRequest(request);
		handler = Router::route(&request, config, *manager);
		HTTPResponse r = handler->handle();
		response = r.buildResponse();
		writeOffset = 0;
	}
}

void Client::onWrite()
{
	CGIHandler* cgiHandler = dynamic_cast<CGIHandler*>(handler);
	if (cgiHandler)
	{
		if (!cgiHandler->isResponseReady()) return;
		response = cgiHandler->getResponseStr();
	}
	if (writeOffset == 0 && response.size())
		loggingMsgOutgoingResponse();
	size_t to_send = (response.size() - writeOffset) < 4096 ? (response.size() - writeOffset) : 4096;
	if (to_send)
	{
		ssize_t n = send(fd, response.c_str() + writeOffset, to_send, 0);
		// verify res
		if (n > 0)
		{
			writeOffset += n;
			if (writeOffset == response.size())
			{
				closed = true;
				delete handler;
				handler = NULL;
			}
		}
		else 
		{
			std::cerr << "Unable to send to [" << this->getIP() << ":" << this->getPort() << "]\n";
			closed = true;
			delete handler;
			handler = NULL;
		}
	}
}

bool Client::isClosed() const
{
	return closed;
}

void Client::setIP(std::string ip)
{
	ipStr = ip;
}

void Client::setPort(std::string port)
{
	portStr = port;
}

std::string Client::getIP() const
{
	return ipStr;
}

std::string Client::getPort() const
{
	return portStr;
}

void Client::loggingMsgIncomingRequest(const HTTPRequest& request) const
{
	std::cout << "* From [" << ipStr << ":" << portStr << "] (" << fd << ") => ";
	if (request.getQuery().empty())
	{
		std::cout << request.getMethod() << " " << request.getPath() << " " << 
		request.getHTTPVersion() << std::endl;
	}
	else
	{
		std::cout << request.getMethod() << " " << request.getPath() << "?" << 
		request.getQuery() << " " << request.getHTTPVersion() << std::endl;
	}
	// std::cout << "\033[35m" << request << "\033[0m" << std::endl;
}

void Client::loggingMsgOutgoingResponse() const
{
	std::cout << "* To   [" << ipStr << ":" << portStr << "] (" << fd << ") => ";
	std::cout << response.substr(0, response.find("\n")) << std::endl;
	// std::cout << response << std::endl;
}
