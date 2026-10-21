#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest() : valid(true) {}

bool HTTPRequest::hasHeader(const std::string &header) const
{
    return headers.count(header) == 1;
}

void HTTPRequest::addHeader(std::string key, std::string value)
{
    this->headers[key] = value;
}

std::string HTTPRequest::getMethod() const
{
    return method;
}

std::string HTTPRequest::getPath() const
{
    return uri;
}

std::string HTTPRequest::getQuery() const
{
    return parameters;
}

std::string HTTPRequest::getHTTPVersion() const
{
    return HTTPVersion;
}

std::map<std::string, std::string> HTTPRequest::getHeaders() const
{
    return headers;
}

std::string HTTPRequest::getBody() const
{
    return body;
}

bool HTTPRequest::isValid() const
{
    return valid;
}

void HTTPRequest::setMethod(std::string token)
{
    this->method = token;
}

void HTTPRequest::setPath(std::string token)
{
    this->uri = token;
}

void HTTPRequest::setQuery(std::string token)
{
    this->parameters = token;
}

void HTTPRequest::setHTTPVersion(std::string token)
{
    this->HTTPVersion = token;
}

void HTTPRequest::setHeaders(std::map<std::string, std::string> token)
{
    this->headers = token;
}

void HTTPRequest::setBody(std::string token)
{
    this->body = token;
}

void HTTPRequest::setValid(bool b)
{
    this->valid = b;
}

void HTTPRequest::setClientIP(const std::string &clientIp)
{
    this->clientIp = clientIp;
}

void HTTPRequest::setclientPort(const std::string &clientPort)
{
    this->clientPort = clientPort;
}

void HTTPRequest::setServertIP(const std::string &serverIp)
{
    this->serverIp = serverIp;
}

void HTTPRequest::setServertPort(const std::string &serverPort)
{
    this->serverPort = serverPort;
}

std::string HTTPRequest::getClientIP() const
{
    return clientIp;
}

std::string HTTPRequest::getClientPort() const
{
    return clientPort;
}

std::string HTTPRequest::getServertIP() const
{
    return serverIp;
}

std::string HTTPRequest::getServertPort() const
{
    return serverPort;
}

std::ostream &operator<<(std::ostream &os, const HTTPRequest &r)
{
    if (r.getQuery().empty())
    {
        os << "< " << r.getMethod() << " " << r.getPath() << " "
        << r.getHTTPVersion() << std::endl;
    }
    else
    {
        os << "< " << r.getMethod() << " " << r.getPath() << "?"
        << r.getQuery() << " " << r.getHTTPVersion() << std::endl;
    }
    
    std::map<std::string, std::string> headers = r.getHeaders();
    std::map<std::string, std::string>::iterator it;
    for (it = headers.begin(); it != headers.end(); ++it)
        os << "< " << it->first <<": " << it->second << std::endl;
    
    os << "< " << (r.getBody().empty() ? "(empty body)" : r.getBody())
        << std::endl << std::endl;

    return os;
}