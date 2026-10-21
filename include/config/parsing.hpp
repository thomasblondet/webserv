#ifndef	PARSING_H
#define	PARSING_H

#define	RESET	"\033[0m"
#define	RED 	"\033[31m"
#define GREEN	"\033[32m"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>
#include "ServerConfig.hpp"

class	Parsing {

private:

	struct	helper {

		int		count_server;
		int		count_braces;

		bool	in_server;
		bool	in_location;

		/* bool for directives */
		bool	listen;
		bool	root;
		bool	autoindex;
		bool	client_max_body_size;
		bool	index;
		int		error_page;
		bool	cgi_handler;

		/* bool for locations */
		bool	l_root;
		bool	l_autoindex;
		bool	l_client_max_body_size;
		bool	l_upload_store;
		bool	l_index;
		int		l_error_page;
		bool	l_cgi_handler;
		bool	l_limit_except;
		bool	l_return;

		std::string	server_name;
		std::string	location_name;
		std::string	error_page_name;
		std::string l_error_page_name;
	};

	struct	s_ServerConfig {

		std::map<std::string, std::vector<std::string> >							directives;
		std::map<std::string, std::map<std::string, std::vector<std::string> > >	location;
	};

	std::vector<ServerConfig>	serverConfigs;

	void	mapToServerConfigsVector();
	bool	isPortValid(const std::string &port) const;
	void	mapToServerConfig(
				std::map<std::string, s_ServerConfig>::const_iterator &it,
				ServerConfig &server);
	void	mapToLocationConfigs(
				std::map<std::string, s_ServerConfig>::const_iterator &map_entry,
				std::vector<LocationConfig> &serverLocations,
				ServerConfig &server);
	void 	setLocationsEmptyAttributes(
				std::vector<LocationConfig> &serverLocations,
				ServerConfig &server);
	bool 	isHTTPMethodValid(const std::string &method) const;
	void	setLocationLimitExcept(
				LocationConfig &location,
				const std::vector<std::string> &methods);
	void	setServerConfigListen(
				ServerConfig &server,
				const std::string &token);
	bool	isClientMaxBodySizeValid(const std::string &sizeStr) const;

public:

	Parsing( void );
	~Parsing( void );

	helper								h;
	std::map<std::string, s_ServerConfig>	s;

	int		check_last_char(std::string &str, bool loop);
	void	parser(std::vector<std::string> &tokens);
	void	parsing(const char *av);
	void	print_map( void );
	std::vector<ServerConfig> getConfig(void) const;


	class	ParseError : public std::exception {

		public:
			virtual const char*	what() const throw();
	};

};

#endif
