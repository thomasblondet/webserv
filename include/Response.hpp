#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"

class Response
{
public:
    Response(const Request &request);
    std::string get_response();
private:
    Request _request;
    std::string _code;
};

#endif