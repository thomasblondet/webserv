#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

class Request
{
public:
    Request(const char *buf);
	Request(const Request &src);
	Request &operator=(const Request &rhs);
    std::string get_method() const;
    std::string get_uri() const;
    std::string get_http_version() const;
    std::map<std::string, std::string> get_headers() const;
    std::string get_body() const;

private:
    std::string _method;
    std::string _uri;
    std::string _http_version;
    std::map<std::string, std::string> _headers;
    std::string _body;

    void handle_status_line(const std::string &line);
    void handle_headers(std::istringstream &iss);
    void handle_body(std::istringstream &iss);
};

#endif
