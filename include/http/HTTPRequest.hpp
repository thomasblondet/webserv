#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
# include <iostream>
# include <ostream>
# include <string>
# include <map>

class HTTPRequest
{
    public:
        HTTPRequest();
        bool hasHeader(const std::string &header) const;
        void addHeader(std::string key, std::string value);
        std::string getMethod() const;
        std::string getPath() const;
        std::string getQuery() const;
        std::string getHTTPVersion() const;
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;
        bool isValid() const;
        void setMethod(std::string token);
        void setPath(std::string token);
        void setQuery(std::string token);
        void setHTTPVersion(std::string token);
        void setHeaders(std::map<std::string, std::string> token);
        void setBody(std::string token);
        void setValid(bool b);
		void setClientIP(const std::string &);
		void setclientPort(const std::string &);
		void setServertIP(const std::string &);
		void setServertPort(const std::string &);
		std::string getClientIP() const;
		std::string getClientPort() const;
		std::string getServertIP() const;
		std::string getServertPort() const;

    private:
        std::string method;
        std::string uri;
        std::string parameters;
        std::string HTTPVersion;
        std::map<std::string, std::string> headers;
        std::string body;
        bool valid;
		std::string	clientIp;
		std::string	clientPort;
		std::string	serverIp;
		std::string	serverPort;
};

std::ostream &operator<<(std::ostream &os, const HTTPRequest &r);

#endif