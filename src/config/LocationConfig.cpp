#include "LocationConfig.hpp"
#include <vector>

LocationConfig::LocationConfig(std::string path)
: path(path), root(""), autoindex(false), unallowed_methods(), index_files(),
client_max_body_size("1000000"), is_there_cgi_handler(false), cgi_extension(""),
cgi_interpreter(""), is_there_redirection(false), redirection_code(""),
redirection_url(""), error_pages(), upload_store("") {}

void LocationConfig::setPath(std::string path)
{
    this->path = path;
}

void LocationConfig::setRoot(std::string root)
{
    this->root = root;
}

void LocationConfig::setListingDirEnable(bool value)
{
    this->autoindex = value;
}

void LocationConfig::setClientMaxBodySize(std::string size)
{
    this->client_max_body_size = size;
}

void LocationConfig::addIndexFile(std::string index_file)
{
    this->index_files.push_back(index_file);
}

void LocationConfig::addUnallowedMethod(std::string method)
{
    this->unallowed_methods.push_back(method);
}

void LocationConfig::setCgiSettings(std::string extension, std::string interpreter)
{
    this->is_there_cgi_handler = true;
    this->cgi_interpreter = interpreter;
    this->cgi_extension = extension;
}

void LocationConfig::setRedirection(std::string code)
{
    this->is_there_redirection = true;
    this->redirection_code = code;
}

void LocationConfig::setRedirection(std::string code, std::string url)
{
    this->is_there_redirection = true;
    this->redirection_code = code;
    this->redirection_url = url;
}

void LocationConfig::addErrorPage(std::string code, std::string url)
{
	this->error_pages[code] = url;
}

void LocationConfig::setUploadStore(std::string upload_store)
{
    this->upload_store = upload_store;
}

std::string	LocationConfig::getPath() const
{
    return path;
}

std::string	LocationConfig::getRoot() const
{
    return root;
}

std::string	LocationConfig::getClientMaxBodySize() const
{
    return client_max_body_size;
}

std::vector<std::string> LocationConfig::getIndexFiles() const
{
    return index_files;
}

bool LocationConfig::isListingDirEnable() const
{
    return autoindex;
}

std::vector<std::string> LocationConfig::getAllowedMethods() const
{
    std::vector<std::string> allowed_methods;
	allowed_methods.push_back("GET");
	allowed_methods.push_back("POST");
	allowed_methods.push_back("DELETE");

	std::vector<std::string>::iterator it = allowed_methods.begin();
	while (it != allowed_methods.end())
	{
		if (std::find(unallowed_methods.begin(), unallowed_methods.end(), *it)
		        != unallowed_methods.end())
			it = allowed_methods.erase(it);
		else
			++it;
	}

    return allowed_methods;
}

bool LocationConfig::isMethodAllowed(std::string method) const
{
    std::vector<std::string> allowed_methods = getAllowedMethods();
    if (std::find(allowed_methods.begin(), allowed_methods.end(), method) != allowed_methods.end())
        return true;
    return false;
}

bool LocationConfig::isThereCgiHandler() const
{
    return is_there_cgi_handler;
}

std::string	LocationConfig::getCgiExtension() const
{
    return cgi_extension;
}

std::string	LocationConfig::getCgiInterpreter() const
{
    return cgi_interpreter;
}

bool LocationConfig::isThereRedirection() const
{
    return is_there_redirection;
}

std::string	LocationConfig::getRedirectionCode() const
{
    return redirection_code;
}

std::string	LocationConfig::getRedirectionUrl() const
{
    return redirection_url;
}

bool LocationConfig::isthereErrorPage(std::string code) const
{
    return error_pages.find(code) != error_pages.end();
}

std::string	LocationConfig::getErrorPage(std::string code) const
{
    return error_pages.find(code)->second;
}

const std::map<std::string, std::string> &LocationConfig::getErrorPages() const
{
    return error_pages;
}

bool LocationConfig::isThereUploadstore() const
{
    return upload_store.length();
}

std::string	LocationConfig::getUploadStore() const
{
    return upload_store;
}

/*
example:

location / {
	root: /var/www/
	autoindex: false
	client_max_body_size: 3k
	index_files: [index.html]
	allowed_methods: [GET, DELETE]
	cgi_settings: [.py, /usr/bin/python3.10]
	return: [301, /home/]
	error_page: [(401 : /error/40x.html) (501 : /error/50x.html) ]
	upload_store: ~/Downloads/
}
*/

std::ostream &operator<<(std::ostream& out, const LocationConfig& obj)
{
	out << "location " << obj.getPath() << " {" << std::endl;
	out << "\troot: " << obj.getRoot() << std::endl;
	out << "\tautoindex: " << (obj.isListingDirEnable() ? "true"  : "false") << std::endl;
	out << "\tclient_max_body_size: " << obj.getClientMaxBodySize() << std::endl;
	out << "\tindex_files: [";
	for (unsigned int i = 0; i < obj.getIndexFiles().size(); i++)
	{
		out << obj.getIndexFiles()[i];
		if (i < obj.getIndexFiles().size() - 1) {out << ", ";}
	}
	out << "]" << std::endl;
	out << "\tallowed_methods: [";
	for (unsigned int i = 0; i < obj.getAllowedMethods().size(); i++)
	{
		out << obj.getAllowedMethods()[i];
		if (i < obj.getAllowedMethods().size() - 1) {out << ", ";}
	}
	out << "]" << std::endl;
	// cgi_settings
	if (obj.isThereCgiHandler()) {out << "\tcgi_settings: [" << obj.getCgiExtension() << ", " << obj.getCgiInterpreter() << "]" << std::endl;}
	else {out << "\tcgi_settings: None" << std::endl;}
	// return
	if (!obj.isThereRedirection()) {out << "\treturn: None" << std::endl;}
	else if (!obj.getRedirectionUrl().length()) {out << "\treturn: [" << obj.getRedirectionCode() << "]" << std::endl;}
	else {out << "\treturn: [" << obj.getRedirectionCode() << ", " << obj.getRedirectionUrl() << "]" << std::endl;}
	// error_page
	out << "\terror_page: [";
	for (std::map<std::string, std::string>::const_iterator it = obj.getErrorPages().begin(); it != obj.getErrorPages().end(); ++it)
	{
		out << "(" << it->first << " : " << it->second << ") ";
	}
	out << "]" << std::endl;
	// upload_store
	if (obj.isThereUploadstore()) {out << "\tupload_store: " << obj.getUploadStore() << std::endl;}
	else {out << "\tupload_store: None" << std::endl;}
	out << "}" << std::endl;
	return out;
}
