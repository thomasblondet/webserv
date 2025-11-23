#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>

class Request
{
public:
    Request(const char *buf);
    std::string getMethod() const;
    std::string getUrl() const;
    std::string getHttpVersion() const;
    std::unordered_map<std::string, std::string> getHeaders() const;
    std::string getBody() const;

private:
    std::string method;
    std::string url;
    std::string httpVersion;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    void handleStatusLine(const std::string &line);
    void handleHeaders(std::istringstream &iss);
    void handleBody(std::istringstream &iss);
};

#endif
