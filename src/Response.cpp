#include "Response.hpp"

Response::Response(const Request &request)
    : _request(request), _code("200 OK")
{
    if (!(request.get_method() == "GET" || request.get_method() == "POST" || request.get_method() == "DELETE"))
    {
        _code = "501 Not Implemented";
    }
    else if (!(request.get_http_version() == "HTTP/1.0"|| request.get_http_version() == "HTTP/1.1"))
        _code = "505 HTTP Version Not Supported";
}

std::string Response::get_response()
{
    std::stringstream ss;
    std::string response;

    ss << _request.get_http_version() << " " << _code << "\r\n\r\n";

    if (_code == "200 OK")
    {
        std::ifstream ifs("www/index.html");

        ss << ifs.rdbuf();
    }
    response = ss.str();

    return response + "\r\n";
}