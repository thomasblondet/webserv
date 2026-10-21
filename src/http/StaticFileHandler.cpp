#include "StaticFileHandler.hpp"

StaticFileHandler::StaticFileHandler(const std::string &p) : path(p) {}

StaticFileHandler::~StaticFileHandler() {}

HTTPResponse StaticFileHandler::handle()
{
    HTTPResponse response;

    if (!Helpers::fileExist(path))
        return Helpers::error(404);
    else if (!Helpers::hasAccess(path))
        return Helpers::error(403);

    try
    {
        response.setBody(readFile(path));
    }
    catch (int statusCode)
    {
        return Helpers::error(statusCode);
    }

    response.setStatus(StatusLine::get(200));
    response.setHeader("Connection", "close");

    return response;
}

/*
    You are not required to use poll() (or an equivalent function) for regular
    disk files;
    read() and write() on them do not require readiness notifications.
*/
std::string readFile(const std::string &path)
{
    std::string body;

    std::ifstream input(path.c_str(), std::ios::in | std::ios::binary);
    if (!input)
        throw 404;

    input.seekg(0, std::ios::end);
    size_t sz = input.tellg();
    input.seekg(0, std::ios::beg);

    std::vector<char> buf(sz);
    if (sz > 0 && input.read(&buf[0], sz))
        body = std::string(&buf[0], sz);
    else
        throw 500;

    return body;
}