#include "DeleteHandler.hpp"

DeleteHandler::DeleteHandler(const std::string &path) : path(path) {}

DeleteHandler::~DeleteHandler() {}

HTTPResponse DeleteHandler::handle()
{
	HTTPResponse response;

	if (Helpers::fileExist(path))
	{
		if (std::remove(path.c_str()) != 0)
			return Helpers::error(403);
	}
	else
		return Helpers::error(404);

	if (!Helpers::fileExist(path))
	{
		response.setStatus(StatusLine::get(200));
		response.setBody(path + " deleted");
		response.setHeader("Connection", "close");
	}
	else
		return Helpers::error(404);

	return response;
}