#ifndef ROUTER_HPP
# define ROUTER_HPP
# include "HTTPRequest.hpp"
# include "ServerConfig.hpp"
# include "IRequestHandler.hpp"
# include "StaticFileHandler.hpp"
# include "ErrorHandler.hpp"
# include "PostHandler.hpp"
# include "DeleteHandler.hpp"
# include "CGIHandler.hpp"
# include "Helpers.hpp"
# include "DirectoryHandler.hpp"
#include "RedirectionHandler.hpp"
#include "EventManager.hpp"

class Router
{
    public:
        static IRequestHandler *route(
            HTTPRequest *request, ServerConfig *config, EventManager &manager);

    private:
        Router();
};
    
LocationConfig *longestPrefixMatch(const std::string &uri, ServerConfig *config);
std::string resolvePath(HTTPRequest *request, LocationConfig *location);

#endif