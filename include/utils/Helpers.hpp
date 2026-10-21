#ifndef HELPERS_HPP
# define HELPERS_HPP
# include "StatusLine.hpp"
# include "HTTPResponse.hpp"
# include <string>
# include <sys/stat.h>
# include <unistd.h>

namespace Helpers
{
    bool fileExist(const std::string &path);
    bool isDirectory(const std::string &path);
    bool hasAccess(const std::string &path);
    HTTPResponse error(int statusCode);
    bool endsWith(const std::string &path, const std::string &ext);
}

#endif