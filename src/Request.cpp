#include "Request.hpp"

Request::Request(const char *buf)
{
    std::string line;

	std::istringstream iss((std::string(buf)));
    if (!iss)
        throw std::runtime_error("istream");

    std::getline(iss, line);
    if (line.empty())
        throw std::runtime_error("No status line");

    handleStatusLine(line);
    //handleHeaders(iss); // WIP
    handleBody(iss);
}

void Request::handleStatusLine(const std::string &line)
{
    std::istringstream iss(line);
    if (!iss)
        throw std::runtime_error("istringstream");

    std::string token;
    iss >> token;
    if (token.empty())
        throw std::runtime_error("No method");
    method = token;

    iss >> token;
    if (token.empty())
        throw std::runtime_error("No url");
    url = token;

    iss >> token;
    if (token.empty())
        throw std::runtime_error("No http version");
    httpVersion = token;
}

// WIP: need more context
void Request::handleHeaders(std::istringstream &iss)
{
    std::string line;

    while (std::getline(iss, line))
    {
        if (line.empty())
            break;

        std::istringstream line_stream(line);
        std::string token, temp;

        line_stream >> temp;
        if (temp.empty())
            throw std::runtime_error("headers");
        if (temp.back() == ':')
            temp = temp.substr(0, temp.size() - 1);

        line_stream >> token;
        if (temp.empty())
            throw std::runtime_error("headers");

        headers[temp] = token;
    }
}

void Request::handleBody(std::istringstream &iss)
{
    std::stringstream buf;

    buf << iss.rdbuf();
    std::string str = buf.str();
    if (str.empty())
        return;
    else
        body = str;
}

std::string Request::getMethod() const
{
    return method;
}

std::string Request::getUrl() const
{
    return url;
}

std::string Request::getHttpVersion() const
{
    return httpVersion;
}

std::unordered_map<std::string, std::string> Request::getHeaders() const
{
    return headers;
}

std::string Request::getBody() const
{
    return body;
}
