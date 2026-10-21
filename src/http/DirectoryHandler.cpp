#include "DirectoryHandler.hpp"
#include <ctime>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ErrorHandler.hpp"
#include "StaticFileHandler.hpp"

DirectoryHandler::DirectoryHandler(
	const std::string path,
	const LocationConfig &location):
	_path(path), 
	_location(location)
{}

DirectoryHandler::~DirectoryHandler() {}

HTTPResponse DirectoryHandler::handle()
{
	if (!_location.getIndexFiles().empty())
	{
		for (unsigned int i = 0; i < _location.getIndexFiles().size(); ++i)
		{
			std::string filePath = _path + _location.getIndexFiles()[i];
			StaticFileHandler fileHandler(filePath);
			HTTPResponse response = fileHandler.handle();
			if (response.getStatusLine().find("200") != std::string::npos)
				return response;
		}
	}

	if (_location.isListingDirEnable())
		return handleListing();

	ErrorHandler errorHandler(403);
	return errorHandler.handle();
}

HTTPResponse DirectoryHandler::handleListing()
{
	DIR *directory = opendir(_path.c_str());
	if (!directory)
	{
		ErrorHandler errorHandler(403);
		return errorHandler.handle();
	}
	HTTPResponse response;
	response.setStatus(StatusLine::get(200));
	constructHeader(response);
	constructBody(response, _location.getPath(), _path, *directory);
	closedir(directory);
	return response;
}

void DirectoryHandler::constructHeader(HTTPResponse &response)
{
	response.setHeader("Server", "webserv/1.0");
	response.setHeader("Content-Type", "text/html");
	response.setHeader("Connection", "close");
	response.setHeader("Content-Type", "text/html; charset=UTF-8");
}

void DirectoryHandler::constructBody(
	HTTPResponse &response,
	std::string uri,
	std::string path_to_dir,
	DIR &directory)
{
	std::string body;
	body += "<!DOCTYPE html>\n<html>\n";
	// html head
	body += "<head>\n";
	body += "<title>Index of " + uri + "</title>\n";
	body += "<style>\n";
    body += "body {font-family: system-ui, -apple-system, Segoe UI, Roboto, sans-serif;background: #f6f8fa;padding: 40px;color: #24292f;}\n";
    body += "h1 {margin-bottom: 20px;}\n";
    body += "table {width: 100%;border-collapse: collapse;background: white;border-radius: 8px;overflow: hidden;box-shadow: 0 2px 10px rgba(0,0,0,0.05);}\n";
    body += "thead {background: #f1f3f5;text-align: left;}\n";
    body += "th, td {padding: 12px 16px;}\n";
    body += "th {font-weight: 600;cursor: pointer;user-select: none;}\n";
	body += "th:last-child {text-align: right;}\n";
    body += "tbody tr {border-top: 1px solid #eee;}\n";
    body += "tbody tr:hover {background: #f9fafb;}\n";
    body += "a {text-decoration: none;color: #0969da;word-break: break-all;}\n";
    body += "a:hover {text-decoration: underline;}\n";
    body += ".size {text-align: right;white-space: nowrap;}\n";
    body += ".date {white-space: nowrap;color: #57606a;}\n";
	body += "</style>\n";
	body += "</head>\n";
	// html body
	body += "<body>\n";
	body += "<h1>Index of " + uri + "</h1>\n";
	body += "<table id='fileTable'>\n";
    body += "<thead>\n";
	body += "<tr>\n";
	body += "<th>Name</th>\n";
	body += "<th>Last modified</th>\n";
	body += "<th>Size</th>\n";
	body += "</tr>\n";
    body += "</thead>\n";
	// html body table
	body += "<tbody>\n";
	body += "<tr>\n";
	body += "<td><a href=\"../\">../</a></td>\n";
	body += "<td class=\"date\">-</td>\n";
	body += "<td class=\"size\">-</td>\n";
	body += "</tr>\n";
	struct dirent *entry;
	while ((entry = readdir(&directory)))
	{
		if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
			continue;
		std::string full_path = path_to_dir + entry->d_name;
		struct stat sb;
		if (stat(full_path.c_str(), &sb) != 0) {continue;}
		body += handleEntry(*entry, sb);
	}
    body += "</tbody>\n";
	body += "</table>\n";
	body += "</body>\n";
	body += "</html>\n";
	response.setBody(body);
}

std::string DirectoryHandler::handleEntry(
	struct dirent &entry,
	struct stat &sb)
{
	std::string filename = handleEntryFilename(entry, sb);
	std::string date = formatTime(sb.st_mtime);
	std::string size = handleEntrySize(sb);
	std::string entryHtml;
	entryHtml += "<tr>\n";
	entryHtml += "<td>" + filename + "</td>\n";
	entryHtml += "<td class=\"date\">" + date + "</td>\n";
	entryHtml += "<td class=\"size\">" + size + "</td>\n";
	entryHtml += "</tr>\n";
	return entryHtml;
}

std::string DirectoryHandler::handleEntryFilename(
	struct dirent &entry,
	struct stat &sb)
{
	std::string filename = "<a href=\"";
	filename += entry.d_name;
	if (S_ISDIR(sb.st_mode)) {filename += "/";}
	filename += "\">";
	filename += entry.d_name;
	if (S_ISDIR(sb.st_mode)) {filename += "/";}
	filename += "</a>";
	return filename;
}

std::string DirectoryHandler::formatTime(time_t t)
{
	char buf[64];
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", std::localtime(&t));
	return buf;
}

std::string DirectoryHandler::handleEntrySize(
	struct stat &sb)
{
	std::ostringstream ss;
	ss << sb.st_size;
	std::string size = ss.str();
	if (S_ISDIR(sb.st_mode)) {size = "-";}
	for (int i = size.length(); i < 16; ++i)
		size = " " + size;
	return size;
}

size_t DirectoryHandler::encodedLength(const std::string &str)
{
    size_t len = 0;
    for (size_t i = 0; i < str.length(); ++i)
        len += (str[i] & 0xc0) != 0x80;
    return len;
}
