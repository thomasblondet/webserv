#ifndef POSTHANDLER_HPP
# define POSTHANDLER_HPP
# include "IRequestHandler.hpp"
# include "HTTPResponse.hpp"
# include "HTTPRequest.hpp"
# include "LocationConfig.hpp"
# include "StatusLine.hpp"
# include "ErrorHandler.hpp"
# include "Helpers.hpp"
# include <sys/stat.h>
# include <unistd.h>
# include <fstream>
# include <sstream>

class PostHandler : public IRequestHandler
{
	public:
	 	PostHandler(HTTPRequest *r, LocationConfig *l);
		~PostHandler();
		HTTPResponse handle();

	private:
		std::string path;
		HTTPRequest *request;
		LocationConfig *location;
};

std::string resolveUploadPath(HTTPRequest *request, LocationConfig *location);
void contentLength(HTTPRequest *request, LocationConfig *location);
void createFile(HTTPRequest *request, const std::string &path);
bool isMultiPartRequest(HTTPRequest *request);

#endif