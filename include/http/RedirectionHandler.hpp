#ifndef REDIRECTIONHANDLER_HPP
#define REDIRECTIONHANDLER_HPP

#include "IRequestHandler.hpp"
#include "LocationConfig.hpp"

class RedirectionHandler : public IRequestHandler
{
public:
	RedirectionHandler(const std::string path, const LocationConfig &);
	~RedirectionHandler();
	HTTPResponse handle();

private:
	const std::string		_path;
	const LocationConfig	&_location;

	HTTPResponse handleListing();
};

#endif // REDIRECTIONHANDLER_HPP