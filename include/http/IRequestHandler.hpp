#ifndef IREQUESTHANDLER_HPP
# define IREQUESTHANDLER_HPP
# include "HTTPRequest.hpp"
# include "ServerConfig.hpp"
# include "HTTPResponse.hpp"

class IRequestHandler
{
    public:
        virtual ~IRequestHandler() {};
        virtual HTTPResponse handle() = 0;
};

#endif