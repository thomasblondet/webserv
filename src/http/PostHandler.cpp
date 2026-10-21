#include "PostHandler.hpp"
#include <cstddef>
#include <iostream>
#include <sstream>
#include <limits>

static size_t parseSizeT(const std::string& value)
{
    if (value.empty())
        throw 400;
    if (value[0] == '-' || value[0] == '+')
        throw 400;
    std::stringstream ss(value);
    unsigned long result;
    ss >> result;
    if (ss.fail())
        throw 400;
    char c;
    if (ss >> c)
        throw 400;
    if (result > std::numeric_limits<size_t>::max())
        throw 400;
    return static_cast<size_t>(result);
}

PostHandler::PostHandler(HTTPRequest *r, LocationConfig *l) : request(r), location(l) {}

PostHandler::~PostHandler() {}

HTTPResponse PostHandler::handle()
{
    HTTPResponse response;
    std::string path = resolveUploadPath(request, location);
    // std::cout << "Path: " << path << std::endl;

    try
    {
        contentLength(request, location);
        if (isMultiPartRequest(request))
            throw 415;
        createFile(request, path);
    }
    catch (int statusCode)
    {
        return Helpers::error(statusCode);
    }
    
    response.setStatus(StatusLine::get(201));
    response.setBody("Upload Successful");
    response.setHeader("Connection", "close");
    
    return response;
}

std::string resolveUploadPath(HTTPRequest *request, LocationConfig *location)
{
    std::string upload = location->getUploadStore(); // /upload
    // return location->getRoot() + upload + request->getPath();
    return upload + request->getPath(); 
    
}

void contentLength(HTTPRequest *request, LocationConfig *location)
{
    if (!request->getHeaders().count("Content-Length"))
        throw 411;

    size_t content_len = parseSizeT(
        request->getHeaders()["Content-Length"]
    );

    if (request->getBody().size() != content_len)
        throw 400;

    size_t max_allowed_size = parseSizeT(
        location->getClientMaxBodySize()
    );

    if (request->getBody().size() > max_allowed_size)
        throw 413;
}


void createFile(HTTPRequest *request, const std::string& path)
{
    if (Helpers::isDirectory(path))
    {
        std::cerr << "Post path is a directory\n";
        throw 403;
    }

    std::ofstream output(path.c_str(), std::ios::binary | std::ios::trunc);
    if (!output)
    {
        std::cerr << "Unable to open: " << path.c_str() << "\n";
        throw 403;
    }
    
    if (!(output << request->getBody()))
        throw 500;
}

bool isMultiPartRequest(HTTPRequest *request)
{
    if (!request)
        return false;
    if (request->hasHeader("Content-Type"))
    {
        std::string value = request->getHeaders()["Content-Type"];
        if (value.find("multipart") != std::string::npos)
            return true;    
    }
    return false;
}
