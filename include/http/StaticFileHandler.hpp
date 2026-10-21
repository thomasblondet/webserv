#ifndef STATICFILEHANDLER_HPP
# define STATICFILEHANDLER_HPP
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "IRequestHandler.hpp"
# include "ServerConfig.hpp"
# include "ErrorHandler.hpp"
# include "StatusLine.hpp"
# include "Helpers.hpp"
# include "MIME.hpp"
# include <unistd.h>
# include <sys/stat.h>
# include <iostream>
# include <fstream>
# include <sstream>

class StaticFileHandler : public IRequestHandler
{
    public:
        StaticFileHandler(const std::string &p);
        ~StaticFileHandler();
        HTTPResponse handle();

    private:
        std::string path;
};

std::string readFile(const std::string& path);

#endif