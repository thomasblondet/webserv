#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP
# include "LocationConfig.hpp"
# include <ostream>
# include <string>
# include <vector>
# include <algorithm>
# include <map>

class ServerConfig
{
    public:
    	ServerConfig();
    	void setHost(std::string host);
    	void setPort(std::string port);
    	void setRoot(std::string root);
    	void setClientMaxBodySize(std::string size);
    	void setListingDirEnable(bool);
        void setLocations(std::vector<LocationConfig> locations);
    	void addIndexFile(std::string index_file);
    	void setCgiSettings(std::string extension, std::string interpreter);
    	void addLocation(LocationConfig location);
		void addErrorPage(std::string code, std::string url);
    	std::string getHost() const;
    	std::string	getPort() const;
    	std::string	getRoot() const;
    	std::string	getClientMaxBodySize() const;
    	bool isListingDirEnable() const;
    	std::vector<std::string> getIndexFiles() const;
    	bool isThereCgiHandler() const;
    	std::string getCgiExtension() const;
    	std::string getCgiInterpreter() const;
    	bool isthereErrorPage(std::string code) const;
    	std::string getErrorPage(std::string code) const;
    	const std::map<std::string, std::string> &getErrorPages() const;
    	const std::vector<LocationConfig> &getLocationConfigs() const;

    private:
    	std::string host;
    	std::string port;
    	std::string	root;
        bool autoindex;
    	std::vector<std::string> index_files;
    	std::string	client_max_body_size;
    	bool is_there_cgi_handler;
    	std::string cgi_extension;
    	std::string	cgi_interpreter;
    	std::map<std::string, std::string> error_pages;
    	std::vector<LocationConfig> locations;
};

std::ostream &operator<<(std::ostream&, const ServerConfig&);

#endif