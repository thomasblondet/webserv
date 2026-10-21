#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
: host(""), port(""), root(""), autoindex(false), index_files(),
client_max_body_size("1000000"), is_there_cgi_handler(false),
cgi_extension(""), cgi_interpreter(""), error_pages(),
locations() {}

void ServerConfig::setHost(std::string host)
{ 
    this->host = host;
}

void ServerConfig::setPort(std::string port)
{ 
    this->port = port;
}

void ServerConfig::setRoot(std::string root)
{
    this->root = root;
}

void ServerConfig::setListingDirEnable(bool value)
{
    this->autoindex = value;
}

void ServerConfig::setClientMaxBodySize(std::string size)
{
    this->client_max_body_size = size;
}

void ServerConfig::setLocations(std::vector<LocationConfig> locations)
{
    this->locations = locations;
}

void ServerConfig::addIndexFile(std::string index_file)
{
    this->index_files.push_back(index_file);
}

void ServerConfig::addLocation(LocationConfig location)
{
    this->locations.push_back(location);
}

void ServerConfig::setCgiSettings(std::string extension, std::string interpreter)
{
    this->is_there_cgi_handler = true;
    this->cgi_interpreter = interpreter;
    this->cgi_extension = extension;
}

void ServerConfig::addErrorPage(std::string code, std::string url)
{
	this->error_pages[code] = url;
}

std::string ServerConfig::getHost() const
{
    return host;
}

std::string ServerConfig::getPort() const
{
    return port;
}

std::string ServerConfig::getRoot() const
{
    return root;
}

std::string ServerConfig::getClientMaxBodySize() const
{
    return client_max_body_size;
}

std::vector<std::string> ServerConfig::getIndexFiles() const
{
    return index_files;
}

bool ServerConfig::isListingDirEnable() const
{
    return autoindex;
}

bool ServerConfig::isThereCgiHandler() const
{
    return is_there_cgi_handler;
}

std::string ServerConfig::getCgiExtension() const
{
    return cgi_extension;
}

std::string ServerConfig::getCgiInterpreter() const
{
    return cgi_interpreter;
}

bool ServerConfig::isthereErrorPage(std::string code) const
{
    return error_pages.find(code) != error_pages.end();
}

std::string ServerConfig::getErrorPage(std::string code) const
{
    return error_pages.find(code)->second;
}

const std::map<std::string, std::string> &ServerConfig::getErrorPages() const
{
    return error_pages;
}

const std::vector<LocationConfig> &ServerConfig::getLocationConfigs() const
{
    return locations;
}

std::ostream &operator<<(std::ostream &out, const ServerConfig &obj)
{
	out << "Server {" << std::endl;
	out << "\tlisten: " << obj.getHost() << ":" << obj.getPort() << std::endl;
	out << "\troot: " << obj.getRoot() << std::endl;
	out << "\tautoindex: " << (obj.isListingDirEnable() ? "true" : "false") << std::endl;
	out << "\tclient_max_body_size: " << obj.getClientMaxBodySize() << std::endl;
	out << "\tindex_files: [";
	for (unsigned int i = 0; i < obj.getIndexFiles().size(); i++)
	{
		out << obj.getIndexFiles()[i];
		if (i < obj.getIndexFiles().size() - 1)
		{
			out << ", ";
		}
	}
	out << "]" << std::endl;
	// cgi_settings
	if (obj.isThereCgiHandler())
	{
		out << "\tcgi_settings: [" << obj.getCgiExtension() << ", " << obj.getCgiInterpreter() << "]" << std::endl;
	}
	else
	{
		out << "\tcgi_settings: None" << std::endl;
	}
	// error_page
	out << "\terror_page: [";
	for (std::map<std::string, std::string>::const_iterator it = obj.getErrorPages().begin(); it != obj.getErrorPages().end(); ++it)
	{
		out << "(" << it->first << " : " << it->second << ") ";
	}
	out << "]" << std::endl;
	// location
	for (std::vector<LocationConfig>::const_iterator it = obj.getLocationConfigs().begin(); it != obj.getLocationConfigs().end(); ++it)
	{
		out << *it << std::endl;
	}
	out << "}" << std::endl;
	return out;
}