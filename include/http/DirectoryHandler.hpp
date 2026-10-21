#ifndef DIRECTORYHANDLER_HPP
#define DIRECTORYHANDLER_HPP

#include "IRequestHandler.hpp"
#include "LocationConfig.hpp"
#include <dirent.h>

class DirectoryHandler : public IRequestHandler
{
public:
	DirectoryHandler(const std::string path, const LocationConfig &);
	~DirectoryHandler();
	HTTPResponse handle();

private:
	const std::string		_path;
	const LocationConfig	&_location;

	HTTPResponse handleListing();
	void constructHeader(HTTPResponse &response);
	void constructBody(HTTPResponse &response, std::string uri,
		std::string path_to_dir, DIR &directory);
	std::string handleEntry(struct dirent &entry, struct stat &sb);
	std::string handleEntryFilename(struct dirent &entry, struct stat &sb);
	std::string formatTime(time_t t);
	std::string handleEntrySize(struct stat &sb);
	size_t		encodedLength(const std::string &str);
};

#endif // DIRECTORYHANDLER_HPP