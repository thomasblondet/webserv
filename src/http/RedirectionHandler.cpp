#include "RedirectionHandler.hpp"
#include "HTTPResponse.hpp"
#include "Helpers.hpp"
#include "StatusLine.hpp"
#include "StaticFileHandler.hpp"

RedirectionHandler::RedirectionHandler(
	const std::string path,
	const LocationConfig &location):
	_path(path), 
	_location(location)
{}

RedirectionHandler::~RedirectionHandler() {}

HTTPResponse RedirectionHandler::handle()
{
	int statusCode = std::atoi(_location.getRedirectionCode().c_str());
	if (_location.isthereErrorPage(_location.getRedirectionCode().c_str()))
	{
		std::string filePath = _location.getRoot();
		filePath += _location.getErrorPage(_location.getRedirectionCode().c_str());
		// We use the StaticFileHandler, to create a page with the custom error page.
		// If this action succeeds, a response with code 200 is return BUT we
		// want the code to be the one of the error.
		StaticFileHandler fileHandler(filePath);
		HTTPResponse response = fileHandler.handle();
		if (response.getStatusLine().find("200") != std::string::npos)
		{
			response.setStatus(StatusLine::get(statusCode));
			return response;
		}
	}
	if (statusCode >= 400 && statusCode < 600)
		return Helpers::error(statusCode);
	HTTPResponse response;
	response.setStatus(StatusLine::get(statusCode));
	response.setBody("");
	response.setHeader("Connection", "close");
	if (!_location.getRedirectionUrl().empty())
		response.setHeader("Location", _location.getRedirectionUrl());
	return response;
}
