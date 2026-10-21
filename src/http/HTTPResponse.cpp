#include "HTTPResponse.hpp"

std::string HTTPResponse::getStatusLine() const
{
    return statusLine;
}

std::map<std::string, std::string> HTTPResponse::getHeaders() const
{
    return headers;
}

std::string HTTPResponse::getBody() const
{
    return body;
}

void HTTPResponse::setStatus(const std::string &str)
{
    this->statusLine = str;
}

void HTTPResponse::setHeader(const std::string &key, const std::string &value)
{
    this->headers[key] = value;
}

void HTTPResponse::setBody(const std::string &str)
{
    std::stringstream oss;
    oss << str.size();
    this->setHeader("Content-Length", oss.str());
    this->body = str;
}

std::string HTTPResponse::buildResponse()
{
    std::string response;
    
    response.append(this->statusLine);
    response.append("\r\n");
    for (std::map<std::string, std::string>::iterator it = this->headers.begin(); 
        it != headers.end(); ++it)
    {
        response.append(it->first);
        response.append(": ");
        response.append(it->second);
        response.append("\r\n");
    }
    response.append("\r\n");    
    response.append(this->body);

    return response;
}

std::ostream &operator<<(std::ostream &os, const HTTPResponse& r)
{
    os << "> " << r.getStatusLine() << std::endl;

    std::map<std::string, std::string> headers = r.getHeaders();
    std::map<std::string, std::string>::iterator it;
    for (it = headers.begin(); it != headers.end(); ++it)
        os << "> " << it->first << ": " << it->second << std::endl;

    if (r.getBody().empty())
        os << "> (empty body)" << std::endl;
    else
    {
        std::istringstream iss(r.getBody());
        std::string line;
        while (std::getline(iss, line))
            os << "> " << line << std::endl;
    }

    os << std::endl << std::endl;

    return os;
}