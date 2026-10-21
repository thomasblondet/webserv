#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP
# include "HTTPRequest.hpp"
# include <string>
# include <sstream>
# include <cstdlib>

enum State
{
    STATE_REQUEST_LINE,
    STATE_HEADERS,
    STATE_BODY,
    STATE_FINISHED,
    STATE_ERROR
};

class HTTPParser
{
    public:
        HTTPParser();
        bool parse(const char *data, size_t nrecv);
        void reset();
        HTTPRequest getRequest() const;

    private:
        HTTPRequest request;
        State state;
        std::string buffer;
        size_t contentLength;
        void requestLine(const std::string &line);
        void headers(const std::string &line);
        std::string removeAnchor(std::string url);
        std::string removeQuery(std::string url);
        std::string getQuery(std::string url);
        unsigned char asciiToHex(char c);
        std::string decodePath(const std::string &path);
        bool isTherePathTraversal(const std::string &decodedPath);
};

#endif