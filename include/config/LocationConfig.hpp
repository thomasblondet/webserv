#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP
# include <ostream>
# include <string>
# include <vector>
# include <algorithm>
# include <map>

class LocationConfig
{
    public:
    	LocationConfig(std::string path);
    	void setPath(std::string path);
    	void setRoot(std::string root);
    	void setClientMaxBodySize(std::string size);
    	void setListingDirEnable(bool);
    	void addIndexFile(std::string index_file);
    	void addUnallowedMethod(std::string method);
    	void setCgiSettings(std::string extension, std::string interpreter);
    	void setRedirection(std::string code);
    	void setRedirection(std::string code, std::string url);
    	void addErrorPage(std::string code, std::string url);
    	void setUploadStore(std::string upload_store);
    	std::string getPath() const;
    	std::string	getRoot() const;
    	std::string	getClientMaxBodySize() const;
    	bool isListingDirEnable() const;
    	std::vector<std::string> getIndexFiles() const;
    	std::vector<std::string> getAllowedMethods() const;
    	bool isMethodAllowed(std::string method) const;
    	bool isThereCgiHandler() const;
    	std::string	getCgiExtension() const;
    	std::string	getCgiInterpreter() const;
    	bool isThereRedirection() const;
    	std::string getRedirectionCode() const;
    	std::string	getRedirectionUrl() const;
    	bool isthereErrorPage(std::string code) const;
    	std::string getErrorPage(std::string code) const;
    	const std::map<std::string, std::string> &getErrorPages() const;
    	bool isThereUploadstore() const;
    	std::string getUploadStore() const;

    private:
    	std::string path;
    	std::string root;
    	bool autoindex;
    	std::vector<std::string> unallowed_methods;
    	std::vector<std::string> index_files;
    	std::string client_max_body_size;
    	bool is_there_cgi_handler;
    	std::string	cgi_extension;
    	std::string	cgi_interpreter;
    	bool is_there_redirection;
    	std::string redirection_code;
    	std::string redirection_url;
    	std::map<std::string, std::string> error_pages;
    	std::string upload_store;
};

std::ostream &operator<<(std::ostream&, const LocationConfig&);

#endif

/*
#include <iostream>
int main()
{
	{
		LocationConfig location("/");
		location.setRoot("/var/www/");
		location.setListingDirEnable(false);
		location.addUnallowedMethod("POST");
		location.addIndexFile("index.html");
		location.setClientMaxBodySize("3k");
		location.setCgiSettings(".py", "/usr/bin/python3.10");
		location.setRedirection("301", "/home/");
		location.addErrorPage("401", "/error/40x.html");
		location.addErrorPage("402", "/error/40x.html");
		location.addErrorPage("403", "/error/40x.html");
		location.addErrorPage("501", "/error/50x.html");
		location.setUploadStore("~/Downloads/");
		std::cout << location << std::endl;
	}
}
*/