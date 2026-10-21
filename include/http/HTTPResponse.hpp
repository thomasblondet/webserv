#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP
# include <iostream>
# include <sstream>
# include <string>
# include <map>

class HTTPResponse
{
    public:
        std::string getStatusLine() const;
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;
        void setStatus(const std::string &str);
        void setHeader(const std::string &key, const std::string &value);
        void setBody(const std::string &str);
        std::string buildResponse();

    private:
        std::string statusLine;
        std::map<std::string, std::string> headers;
        std::string body;
};

std::ostream &operator<<(std::ostream &os, const HTTPResponse &r);

#endif