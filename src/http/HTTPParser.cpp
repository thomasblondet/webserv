#include "HTTPParser.hpp"

HTTPParser::HTTPParser()
    : state(STATE_REQUEST_LINE), contentLength(0)
{
    request.setValid(true);
}

bool HTTPParser::parse(const char *data, size_t nrecv)
{
    if (state == STATE_FINISHED || state == STATE_ERROR)
        return true;
    
    buffer.append(data, nrecv);

    while (state != STATE_BODY && state != STATE_FINISHED
        && state != STATE_ERROR)
    {
        size_t pos = buffer.find("\r\n");
        if (pos == std::string::npos)
            break;
        
        std::string line = buffer.substr(0, pos);
        buffer.erase(0, pos + 2);

        if (state == STATE_REQUEST_LINE)
            requestLine(line);
        else if (state == STATE_HEADERS)
        {
            // No more headers
            if (line.empty())
            {
                if (request.getHeaders().count("Content-Length"))
                {
                    std::string len = request.getHeaders()["Content-Length"];
                    contentLength = std::atoi(len.c_str());
                    state = STATE_BODY;
                }
                else
                {
                    state = STATE_FINISHED;
                    return true;
                }
            }
            else
                headers(line);
        }
    }

    if (state == STATE_BODY)
    {
        if (buffer.size() >= contentLength)
        {
            request.setBody(buffer.substr(0, contentLength));
            buffer.erase(0, contentLength);
            state = STATE_FINISHED;
            return true;
        }
    }

    return state == STATE_FINISHED || state == STATE_ERROR;
}

void HTTPParser::requestLine(const std::string &line)
{
    std::istringstream iss(line);

    std::string method, url, version;
    if (!(iss >> method >> url >> version))
    {
        state = STATE_ERROR;
        request.setValid(false);
        return;
    }

    request.setMethod(method);
    std::string decodedPath = decodePath(removeQuery(url));
	if (decodedPath.empty() || isTherePathTraversal(decodedPath))
    {
        state = STATE_ERROR;
        request.setValid(false);
        return;
    }
    request.setPath(decodedPath);
    request.setQuery(getQuery(url));
    request.setHTTPVersion(version);

    if (request.getHTTPVersion() != "HTTP/1.0"
        && request.getHTTPVersion() != "HTTP/1.1")
    {
        state = STATE_ERROR;
        request.setValid(false);
        return;
    }

    state = STATE_HEADERS;
}

void HTTPParser::headers(const std::string &line)
{
    size_t colon = line.find(':');
    if (colon != std::string::npos)
    {
        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon + 1);

        // Trim spaces
        size_t first = value.find_first_not_of(" \t");
        if (first != std::string::npos)
            value = value.substr(first);
        
        request.addHeader(key, value);
    }
}

void HTTPParser::reset()
{
    state = STATE_REQUEST_LINE;
    buffer.clear();
    request = HTTPRequest();
    contentLength = 0;
}

HTTPRequest HTTPParser::getRequest() const
{
    return request;
}

std::string HTTPParser::removeAnchor(std::string url)
{
    return url.substr(0, url.find("#"));
}

std::string HTTPParser::removeQuery(std::string url)
{
    url = removeAnchor(url);
    return url.substr(0, url.find("?"));
}

std::string HTTPParser::getQuery(std::string url)
{
    url = removeAnchor(url);
    size_t pos = url.find("?");
	if (pos != std::string::npos)
        return url.substr(pos + 1);
    return "";
}

unsigned char HTTPParser::asciiToHex(char c)
{
	unsigned char hex;
	if (std::isdigit(c))
		hex = c - '0'; 
	else if (c < 'a')
		hex = c - 'A' + 10;
	else
		hex = c - 'a' + 10;
	return hex;
}

/*
    return an empty string in case of error.
*/
std::string HTTPParser::decodePath(const std::string &path)
{
	std::string decodedPath;
	for (size_t i = 0; i < path.size(); ++i)
	{
		if (path[i] == '%')
		{
			if (!std::isxdigit(path[i + 1]) || !std::isxdigit(path[i + 2]))
				return "";
			unsigned char hex = asciiToHex(path[i + 1]);
			hex = hex << 4;
			hex = hex | asciiToHex(path[i + 2]);
			if (hex == 0)
				return "";
			decodedPath += hex;
			i += 2;
		}
		else decodedPath += path[i];
	}
	return decodedPath;
}

bool HTTPParser::isTherePathTraversal(const std::string &decodedPath)
{
	if (decodedPath.find("../") != std::string::npos)
		return true;
	if (decodedPath.find("/..") != std::string::npos)
		return true;
	return false;
}
