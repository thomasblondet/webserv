#ifndef ERRORHANDLER_HPP
# define ERRORHANDLER_HPP
# include "IRequestHandler.hpp"
# include "HTTPRequest.hpp"
# include "ServerConfig.hpp"
# include "HTTPResponse.hpp"
# include "StatusLine.hpp"
# include <iostream>
# include <string>
# include <map>

class ErrorHandler : public IRequestHandler
{
    public:
        ErrorHandler(int code);
        ~ErrorHandler();
        HTTPResponse handle();
        static HTTPResponse makeErrorResponse(int statusCode);

    private:
        int statusCode;
};

#endif