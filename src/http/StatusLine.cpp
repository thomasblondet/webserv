#include "StatusLine.hpp"

std::string StatusLine::get(int status_code)
{
    std::map<int, std::string> reason_phrase;

    /* 2xx Success */
    reason_phrase[200] = "HTTP/1.0 200 OK";
    reason_phrase[201] = "HTTP/1.0 201 Created";
    reason_phrase[202] = "HTTP/1.0 202 Accepted";
    reason_phrase[204] = "HTTP/1.0 204 No Content";

    /* 3xx Redirection */
    reason_phrase[301] = "HTTP/1.0 301 Moved Permanently";
    reason_phrase[302] = "HTTP/1.0 302 Found";
    reason_phrase[303] = "HTTP/1.0 303 See Other";
    reason_phrase[304] = "HTTP/1.0 304 Not Modified";
    reason_phrase[307] = "HTTP/1.0 307 Temporary Redirect";

    /* 4xx Client Error */
    reason_phrase[400] = "HTTP/1.0 400 Bad Request";
    reason_phrase[401] = "HTTP/1.0 401 Unauthorized";
    reason_phrase[403] = "HTTP/1.0 403 Forbidden";
    reason_phrase[404] = "HTTP/1.0 404 Not Found";
    reason_phrase[405] = "HTTP/1.0 405 Method Not Allowed";
    reason_phrase[408] = "HTTP/1.0 408 Request Timeout";
    reason_phrase[411] = "HTTP/1.0 411 Length Required";
    reason_phrase[413] = "HTTP/1.0 413 Payload Too Large";
    reason_phrase[414] = "HTTP/1.0 414 URI Too Long";
    reason_phrase[415] = "HTTP/1.0 415 Unsupported Media Type";

    /* 5xx Server Error */
    reason_phrase[500] = "HTTP/1.0 500 Internal Server Error";
    reason_phrase[501] = "HTTP/1.0 501 Not Implemented";
    reason_phrase[502] = "HTTP/1.0 502 Bad Gateway";
    reason_phrase[503] = "HTTP/1.0 503 Service Unavailable";
    reason_phrase[504] = "HTTP/1.0 504 Gateway Timeout";


    return reason_phrase.count(status_code) ? reason_phrase[status_code]
        : reason_phrase[500];
}