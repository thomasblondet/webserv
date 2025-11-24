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

    handle_status_line(line);
    //handle_headers(iss); // WIP
    handle_body(iss);
}

Request::Request(const Request &src)
{
	*this = src;
}

Request &Request::operator=(const Request &rhs)
{
	if (this != &rhs)
	{
		_method = rhs.get_method();
		_uri = rhs.get_uri();
		_http_version = rhs.get_http_version();
		_headers = rhs.get_headers();
		_body = rhs.get_body();
	}
	return *this;
}

void Request::handle_status_line(const std::string &line)
{
    std::istringstream iss(line);
    if (!iss)
        throw std::runtime_error("istringstream");

    std::string token;
    iss >> token;
    if (token.empty())
        throw std::runtime_error("No method");
    _method = token;

    iss >> token;
    if (token.empty())
        throw std::runtime_error("No uri");
    _uri = token;

    iss >> token;
    if (token.empty())
        throw std::runtime_error("No http version");
    _http_version = token;
}

// WIP: need more context
void Request::handle_headers(std::istringstream &iss)
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

        _headers[temp] = token;
    }
}

void Request::handle_body(std::istringstream &iss)
{
    std::stringstream buf;

    buf << iss.rdbuf();
    std::string str = buf.str();
    if (str.empty())
        return;
    else
        _body = str;
}

std::string Request::get_method() const
{
    return _method;
}

std::string Request::get_uri() const
{
    return _uri;
}

std::string Request::get_http_version() const
{
    return _http_version;
}

std::map<std::string, std::string> Request::get_headers() const
{
    return _headers;
}

std::string Request::get_body() const
{
    return _body;
}
