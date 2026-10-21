#include "Router.hpp"
#include "ServerConfig.hpp"
#include <cstddef>
#include <sstream>

IRequestHandler* Router::route(
	HTTPRequest* request, ServerConfig* config, EventManager &manager)
{
	LocationConfig* location = longestPrefixMatch(request->getPath(), config);
	if (!location)
		return new ErrorHandler(404);

	std::string path;
	try
	{
		path = resolvePath(request, location);
	}
	catch (int status_code)
	{
		return new ErrorHandler(status_code);
	}
	std::cout << "  path    : " << path << "\n";
	std::cout << "  location: " << location->getPath() << "\n";
	request->setServertIP(config->getHost());
	request->setServertPort(config->getPort());

	if (!request->isValid())
		return new ErrorHandler(400);

	if (!location->isMethodAllowed(request->getMethod()))
		return new ErrorHandler(405);

	std::size_t maxBodySize;
    std::stringstream ss(location->getClientMaxBodySize());
    ss >> maxBodySize;
	if (request->getBody().size() > maxBodySize)
		return new ErrorHandler(413);

	if (request->getMethod() == "POST")
	{
		if (location->isThereCgiHandler() 
			&& Helpers::endsWith(path, location->getCgiExtension()))
			return new CGIHandler(path, *request, *location, manager);
		return new PostHandler(request, location);
	}
	else if (request->getMethod() == "DELETE")
		return new DeleteHandler(path);

	if (location->isThereRedirection())
		return new RedirectionHandler(path, *location);

	if (!Helpers::fileExist(path) && !Helpers::isDirectory(path))
		return new ErrorHandler(404);

	if (Helpers::isDirectory(path))
		return new DirectoryHandler(path, *location);

	if (location->isThereCgiHandler()
		&& Helpers::endsWith(path, location->getCgiExtension()))
		return new CGIHandler(path, *request, *location, manager);

	return new StaticFileHandler(path);
}

std::string resolvePath(HTTPRequest* request, LocationConfig* location)
{
	std::string uri = request->getPath();
	std::string root = location->getRoot();
	std::string path = location->getPath();
   
	if (uri.find("..") != std::string::npos)
		throw 403;
	
	if (uri == "/")
	{
		for (size_t i = 0; i < location->getIndexFiles().size(); ++i)
		{
			if (Helpers::fileExist(root + "/" + location->getIndexFiles()[i]))
				return root + "/" + location->getIndexFiles()[i];
		}
	}
	
	return root + uri;
}

LocationConfig* longestPrefixMatch(const std::string &uri, ServerConfig* config)
{
	LocationConfig *best = NULL;
	size_t best_len = 0;

	for (size_t i = 0; i < config->getLocationConfigs().size(); ++i)
	{
		std::string path = config->getLocationConfigs()[i].getPath();
		if (uri.compare(0, path.size(), path) == 0 && path.size() > best_len)
		{
			best = const_cast<LocationConfig*>(&config->getLocationConfigs()[i]);
			best_len = path.size();
		}
		if (*uri.rbegin() != '/')
		{
			std::string tempUri = uri + "/";
			if (tempUri.compare(0, path.size(), path) == 0 && path.size() > best_len)
			{
				best = const_cast<LocationConfig*>(&config->getLocationConfigs()[i]);
				best_len = path.size();
			}
		}
	}
	return best;
}