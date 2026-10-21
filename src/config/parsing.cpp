#include "parsing.hpp"
#include "LocationConfig.hpp"
#include <cstddef>
#include <map>
#include <vector>

Parsing::Parsing( void ) {

	h.count_server = 0;
	h.count_braces = 0;

	h.in_server = false;
	h.in_location = false;

	h.listen = false;
	h.root = false;
	h.autoindex = false;
	h.client_max_body_size = false;
	h.index = false;
	h.error_page = 0;
	h.cgi_handler = false;

	h.l_root = false;
	h.l_autoindex = false;
	h.l_client_max_body_size = false;
	h.l_upload_store = false;
	h.l_index = false;
	h.l_error_page = 0;
	h.l_cgi_handler = false;
	h.l_limit_except = false;
	h.l_return = false;

	return ;
}

Parsing::~Parsing( void ) {

	return ;
}

int	Parsing::check_last_char(std::string &str, bool loop) {

	if (loop == false) {

		if (str[str.size() - 1] == ';')
			str.erase(str.size() - 1);
		else {
	
			std::cerr << RED << "Missing ';' on a directive inside server[" << h.count_server << "]." << RESET << std::endl;
			throw ParseError();
		}
	}
	else if (loop == true) {

		if (str[str.size() - 1] == ';') {

			std::cerr << RED << "Token \"" << str << "\" shouldn't be with ';' at the end" << RESET << std::endl;
			throw ParseError();
		}
	}
	return 1;
}

void	Parsing::parser(std::vector<std::string> &tokens) {

	if (tokens.empty())
		return ;

	int	e = tokens.size();

	if (tokens[0] == "server" && e == 2 && tokens[1] == "{" && h.in_server == false) { //begin of server

		h.count_server++;
		h.count_braces++;
		h.in_server = true;
		std::ostringstream oss;
		oss << "server_" << h.count_server;
		h.server_name = oss.str();
		s[h.server_name];
	}
	else if (tokens[0] == "}" && h.in_location == false && h.in_server == true && e == 1) { //end of server

		h.count_braces--;
		h.in_server = false;

		h.listen = false;
		h.root = false;
		h.autoindex = false;
		h.client_max_body_size = false;
		h.index = false;
		h.error_page = 0;
		h.cgi_handler = false;
	}
	else if (h.in_server == true) {

		if (h.in_location == false) {

			if ((tokens[0] == "listen" || tokens[0] == "root" || tokens[0] == "autoindex" || tokens[0] == "client_max_body_size" )
				&& e == 2 && check_last_char(tokens[1], false)) { //directive 1 arguments

				if (tokens[0] == "listen" && h.listen == false) {

					h.listen = true;
					s[h.server_name].directives[tokens[0]].push_back(tokens[1]);
				}
				else if (tokens[0] == "root" && h.root == false) {

					h.root = true;
					s[h.server_name].directives[tokens[0]].push_back(tokens[1]);
				}
				else if (tokens[0] == "autoindex" && h.autoindex == false) {

					if (tokens[1] != "on" && tokens[1] != "off") {

						std::cerr << RED << "autoindex should be either \"on\" or \"off\" inside server[" << h.in_server << "]." << RESET << std::endl;
						throw ParseError();
					}
					h.autoindex = true;
					s[h.server_name].directives[tokens[0]].push_back(tokens[1]);
				}
				else if (tokens[0] == "client_max_body_size" && h.client_max_body_size == false) {

					h.client_max_body_size = true;
					s[h.server_name].directives[tokens[0]].push_back(tokens[1]);
				}
				else {

					std::cerr << RED << "Found duplicate inside server[" << h.in_server << "]." << RESET << std::endl;
					throw ParseError();
				}
			}
			else if (tokens[0] == "index" && e >= 2) {

				if (h.index == false) {

					for (int i = 1; i < e - 1; i++) {

						if (check_last_char(tokens[i], true))
							s[h.server_name].directives[tokens[0]].push_back(tokens[i]);
					}
					if (check_last_char(tokens[e - 1], false))
						s[h.server_name].directives[tokens[0]].push_back(tokens[e - 1]);
					h.index = true;
				}
				else {

					std::cerr << RED << "Found duplicate inside server[" << h.in_server << "]." << RESET << std::endl;
					throw ParseError();
				}
			}
			else if (tokens[0] == "error_page" && e == 3) {

				h.error_page++;
				std::ostringstream oss;
				oss << "error_page_" << h.error_page;
				h.error_page_name = oss.str();
				if (check_last_char(tokens[1], true))
					s[h.server_name].directives[h.error_page_name].push_back(tokens[1]);
				if (check_last_char(tokens[2], false))
					s[h.server_name].directives[h.error_page_name].push_back(tokens[2]);
			}
			else if (tokens[0] == "cgi_handler" && e == 3) {

				if (h.cgi_handler == false) {

					if (tokens[1][0] != '.') {

						std::cerr << RED << "wrong cgi_handler arguments inside server[" << h.in_server << "]." << RESET << std::endl;
						throw ParseError();
					}
					if (check_last_char(tokens[1], true))
						s[h.server_name].directives[tokens[0]].push_back(tokens[1]);
					if (check_last_char(tokens[2], false))
						s[h.server_name].directives[tokens[0]].push_back(tokens[2]);
					h.cgi_handler = true;
				}
				else {

					std::cerr << RED << "Found duplicate inside server[" << h.in_server << "]." << RESET << std::endl;
					throw ParseError();
				}
			}
			else if (tokens[0] == "location" && e == 3 && tokens[2] == "{") { //location block found

				if (tokens[1][0] != '/') {

					std::cerr << RED << "Wrong path for location inside server[" << h.in_server << "]." << RESET << std::endl;
					throw ParseError();
				}
				if (s[h.server_name].location.find(tokens[1]) != s[h.server_name].location.end()) {

					std::cerr << RED << "Duplicate location '" << tokens[1] << "' inside server[" << h.in_server << "]." << RESET << std::endl;
					throw ParseError();
				}
				h.location_name = tokens[1];
				s[h.server_name].location[h.location_name];
				h.in_location = true;
				h.count_braces++;
			}
			else {

				std::cerr << RED << "Found wrong directives inside server[" << h.in_server << "]." << RESET << std::endl;
				throw ParseError();
			}
		}
		else if (h.in_location == true) { //parse of the location block

			if ((tokens[0] == "root" || tokens[0] == "autoindex" || tokens[0] == "client_max_body_size" || tokens[0] == "upload_store")
				&& e == 2 && check_last_char(tokens[1], false)) {
				
				if (tokens[0] == "root" && h.l_root == false) {

					h.l_root = true;
					s[h.server_name].location[h.location_name][tokens[0]].push_back(tokens[1]);
				}
				else if (tokens[0] == "autoindex" && h.l_autoindex == false) {

					if (tokens[1] != "on" && tokens[1] != "off") {

						std::cerr << RED << "autoindex should be either \"on\" or \"off\" for location inside server[" << h.in_server << "]." << RESET << std::endl;
						throw ParseError();
					}
					h.l_autoindex = true;
					s[h.server_name].location[h.location_name][tokens[0]].push_back(tokens[1]);
				}
				else if (tokens[0] == "client_max_body_size" && h.l_client_max_body_size == false) {

					h.l_client_max_body_size = true;
					s[h.server_name].location[h.location_name][tokens[0]].push_back(tokens[1]);
				}
				else if (tokens[0] == "upload_store" && h.l_upload_store == false) {

					h.l_upload_store = true;
					s[h.server_name].location[h.location_name][tokens[0]].push_back(tokens[1]);
				}
				else {

					std::cerr << RED << "Found duplicate inside location " << h.location_name <<  " inside server[" << h.in_server << "]." << RESET << std::endl;
					throw ParseError();
				}
			}
			else if (tokens[0] == "index" && e >= 2) {

				if (h.l_index == false) {

					for (int i = 1; i < e - 1; i++) {

						if (check_last_char(tokens[i], true))
							s[h.server_name].location[h.location_name][tokens[0]].push_back(tokens[i]);
					}
					if (check_last_char(tokens[e - 1], false))
						s[h.server_name].location[h.location_name][tokens[0]].push_back(tokens[e - 1]);
					h.l_index = true;
				}
				else {

					std::cerr << RED << "Found duplicate inside location " << h.location_name <<  " inside server[" << h.in_server << "]." << RESET << std::endl;
					throw ParseError();
				}
			}
			else if (tokens[0] == "error_page" && e == 3) {

				h.l_error_page++;
				std::ostringstream oss;
				oss << "error_page_" << h.l_error_page;
				h.l_error_page_name = oss.str();
				if (check_last_char(tokens[1], true))
					s[h.server_name].location[h.location_name][h.l_error_page_name].push_back(tokens[1]);
				if (check_last_char(tokens[2], false))
					s[h.server_name].location[h.location_name][h.l_error_page_name].push_back(tokens[2]);
			}
			else if (tokens[0] == "cgi_handler" && e == 3) {

				if (h.l_cgi_handler == false) {

					if (tokens[1][0] != '.') {

						std::cerr << RED << "wrong cgi_handler arguments for location inside server[" << h.in_server << "]." << RESET << std::endl;
						throw ParseError();
					}
					if (check_last_char(tokens[1], true))
						s[h.server_name].location[h.location_name][tokens[0]].push_back(tokens[1]);
					if (check_last_char(tokens[2], false))
						s[h.server_name].location[h.location_name][tokens[0]].push_back(tokens[2]);
					h.l_cgi_handler = true;
				}
				else {

					std::cerr << RED << "Found duplicate inside location " << h.location_name <<  " inside server[" << h.in_server << "]." << RESET << std::endl;
					throw ParseError();
				}
			}
			else if (tokens[0] == "limit_except" && e >= 2) {

				if (h.l_limit_except == false) {

					for (int i = 1; i < e - 1; i++) {

						if (check_last_char(tokens[i], true))
							s[h.server_name].location[h.location_name][tokens[0]].push_back(tokens[i]);
					}
					if (check_last_char(tokens[e - 1], false))
						s[h.server_name].location[h.location_name][tokens[0]].push_back(tokens[e - 1]);
				}
				else {

					std::cerr << RED << "Found duplicate inside location " << h.location_name <<  " inside server[" << h.in_server << "]." << RESET << std::endl;
					throw ParseError();
				}
			}
			else if (tokens[0] == "return" && (e == 2 || e == 3)) {

				if (h.l_return == false) {

					if (e == 2) {

						if (tokens[1].size() != 4) {

							std::cerr << RED << "wrong return arguments for location inside server[" << h.in_server << "]." << RESET << std::endl;
							throw ParseError();
						}
						if (check_last_char(tokens[1], false))
							s[h.server_name].location[h.location_name][tokens[0]].push_back(tokens[1]);
					}
					else if (e == 3) {

						if (tokens[1].size() != 3) {

							std::cerr << RED << "wrong return arguments for location inside server[" << h.in_server << "]." << RESET << std::endl;
							throw ParseError();
						}
						if (check_last_char(tokens[1], true))
							s[h.server_name].location[h.location_name][tokens[0]].push_back(tokens[1]);
						if (check_last_char(tokens[2], false))
							s[h.server_name].location[h.location_name][tokens[0]].push_back(tokens[2]);
					}
					h.l_return = true;
				}
				else {

					std::cerr << RED << "Found duplicate inside location " << h.location_name <<  " inside server[" << h.in_server << "]." << RESET << std::endl;
					throw ParseError();
				}
			}
			else if (tokens[0] == "}" && e == 1) {

				h.in_location = false;
				h.l_root = false;
				h.l_autoindex = false;
				h.l_client_max_body_size = false;
				h.l_upload_store = false;
				h.l_index = false;
				h.l_error_page = 0;
				h.l_cgi_handler = false;
				h.l_limit_except = false;
				h.l_return = false;
				h.count_braces--;
			}
			else {

				std::cerr << RED << "Found wrong directives inside location in server[" << h.in_server << "]." << RESET << std::endl;
				throw ParseError();
			}
		}
	}
	else {

		std::cerr << "Found something wrong in the config file" << std::endl;
		throw ParseError();
	}
}

void	Parsing::parsing(const char *av) {

	std::string	file = av;
	if (file.size() < 5 || file.substr(file.size() - 5) != ".conf") {

		std::cerr << RED << "File must have a .conf extension" << RESET << std::endl;
		throw ParseError();
	}

	std::ifstream	input(file.c_str());
	if (!input) {

		std::cerr << RED << "Cannot open input file" << RESET << std::endl;
		throw ParseError();
	}

	std::string	line;
	while (std::getline(input, line)) {

		const std::string::size_type comment = line.find('#');
		if (comment != std::string::npos)
			line.erase(comment);

		std::vector<std::string>	tokens;
		std::istringstream			iss(line);
		std::string					word;

		while (iss >> word) {

			tokens.push_back(word);
		}
		parser(tokens);
	}
	if (h.count_braces != 0) {

		std::cerr << RED << "Something is wrong with braces inside server[" << h.count_server << "]." << RESET << std::endl;
		throw ParseError();
	}
	input.close();
	mapToServerConfigsVector();
}

void	Parsing::print_map( void ) {

	std::cout << " -----------------------------" << std::endl;
	std::cout << "|             MAP             |" << std::endl;
	std::cout << " -----------------------------" << std::endl;
	for (std::map<std::string, s_ServerConfig>::const_iterator it = s.begin(); it != s.end(); ++it) {

		std::cout << std::endl << "[" << it->first << "]" << std::endl;

		//--- directives ---
		std::cout << "	directives:" << std::endl;
		for (std::map<std::string, std::vector<std::string> >::const_iterator d = it->second.directives.begin(); d!= it->second.directives.end(); ++d) {

			std::cout << GREEN << "		" << d->first << " = ";
			for (size_t i = 0; i < d->second.size(); ++i) {

				std::cout << d->second[i];
				if (i + 1 < d->second.size())
					std::cout << ", ";
			}
			std::cout << RESET << std::endl;
		}

		//--- locations ---
		std::cout << "	locations:" << std::endl;
		for (std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator l = it->second.location.begin(); l != it->second.location.end(); ++l) {

			std::cout << "		location " << l->first << " :" << std::endl;
			for (std::map<std::string, std::vector<std::string> >::const_iterator ld = l->second.begin(); ld != l->second.end(); ++ld) {

				std::cout << GREEN << "		" << ld->first << " = ";
				for (size_t i = 0; i < ld->second.size(); ++i) {

					std::cout << ld->second[i];
					if (i + 1 < ld->second.size())
						std::cout << " ";
				}
				std::cout << RESET << std::endl;
			}
		}
	}
}

const char*	Parsing::ParseError::what( void ) const throw() {

	return "\033[1;33mError Catched\033[0m";
}


void Parsing::mapToServerConfigsVector()
{
	for (
		std::map<std::string, s_ServerConfig>::const_iterator map_entry = s.begin();
		map_entry != s.end();
		++map_entry
	)
	{
		ServerConfig server;
		std::vector<LocationConfig> serverLocations;
		mapToServerConfig(map_entry, server);
		mapToLocationConfigs(map_entry, serverLocations, server);
		setLocationsEmptyAttributes(serverLocations, server);
		for (size_t i = 0; i < serverLocations.size(); ++i)
			server.addLocation(serverLocations[i]);
		serverConfigs.push_back(server);
	}
}

bool Parsing::isPortValid(const std::string &port) const
{
	if (port.empty()) return false;
	if (port.size() > 5) return false;
	int p = std::atoi(port.c_str());
	if (!(p > 0 && p < 65536)) return false;
	size_t i = 0;
	while (i < port.size())
	{
		if (!std::isdigit(port[i])) return false;
		++i;
	}
	return true;
}

std::vector<ServerConfig> Parsing::getConfig(void) const
{
	return serverConfigs;
}

void Parsing::mapToServerConfig(
	std::map<std::string, s_ServerConfig>::const_iterator &map_entry,
	ServerConfig &server)
{
	for (
		std::map<std::string, std::vector<std::string> >::const_iterator
			d = map_entry->second.directives.begin();
		d!= map_entry->second.directives.end();
		++d
	)
	{
		if (d->first == "listen")
			setServerConfigListen(server, d->second[0]);
		else if (d->first == "root")
			server.setRoot(d->second[0]);
		else if (d->first == "autoindex")
		{
			if (d->second[0] == "on") server.setListingDirEnable(true);
			else if (d->second[0] == "off") server.setListingDirEnable(false);
		}
		else if (d->first == "client_max_body_size")
		{
			if (!isClientMaxBodySizeValid(d->second[0]))
			{
				std::cerr << "client_max_body_size: " << d->second[0] << " is not valid\n";
				throw ParseError();
			}
			server.setClientMaxBodySize(d->second[0]);
		}
		else if (d->first == "index")
		{
			for (size_t i = 0; i < d->second.size(); ++i)
				server.addIndexFile(d->second[i]);
		}
		else if (d->first == "cgi_settings")
			server.setCgiSettings(d->second[0], d->second[1]);
		else if (d->first == "error_page")
			server.addErrorPage(d->second[0], d->second[1]);
	}
}

void Parsing::mapToLocationConfigs(
	std::map<std::string, s_ServerConfig>::const_iterator &map_entry,
	std::vector<LocationConfig> &serverLocations,
	ServerConfig &server)
{
	for (
		std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator
			l = map_entry->second.location.begin();
		l != map_entry->second.location.end();
		++l
	)
	{
		LocationConfig location(l->first);
		location.setRoot(server.getRoot());
		location.setListingDirEnable(server.isListingDirEnable());
		location.setClientMaxBodySize(server.getClientMaxBodySize());
		for (
			std::map<std::string, std::vector<std::string> >::const_iterator
				ld = l->second.begin();
			ld != l->second.end();
			++ld
		)
		{
			if (ld->first == "root")
				location.setRoot(ld->second[0]);
			else if (ld->first == "autoindex")
			{
				if (ld->second[0] == "on") location.setListingDirEnable(true);
				else if (ld->second[0] == "off") location.setListingDirEnable(false);
			}
			else if (ld->first == "client_max_body_size")
			{
				if (!isClientMaxBodySizeValid(ld->second[0]))
				{
					std::cerr << "client_max_body_size: " << ld->second[0] << " is not valid\n";
					throw ParseError();
				}
				location.setClientMaxBodySize(ld->second[0]);
			}
			else if (ld->first == "index")
			{
				for (size_t i = 0; i < ld->second.size(); ++i)
					location.addIndexFile(ld->second[i]);
			}
			else if (ld->first == "limit_except")
				setLocationLimitExcept(location, ld->second);
			else if (ld->first == "cgi_handler")
				location.setCgiSettings(ld->second[0], ld->second[1]);
			else if (ld->first == "return")
			{
				if (ld->second.size() == 2)
					location.setRedirection(ld->second[0], ld->second[1]);
				else
				 	location.setRedirection(ld->second[0]);
			}
			else if (ld->first.substr(0,10) == "error_page")
				location.addErrorPage(ld->second[0], ld->second[1]);
			else if (ld->first == "upload_store")
				location.setUploadStore(ld->second[0]);
		}
		serverLocations.push_back(location);
	}
}

void Parsing::setLocationsEmptyAttributes(
	std::vector<LocationConfig> &serverLocations,
	ServerConfig &server)
{
	for (size_t i = 0; i < serverLocations.size(); ++i)
	{
		if (serverLocations[i].getIndexFiles().empty())
		{
			std::vector<std::string> indexFiles = server.getIndexFiles();
			for (size_t j = 0; j < indexFiles.size(); ++j)
				serverLocations[i].addIndexFile(indexFiles[j]);
		}
		if (!serverLocations[i].isThereCgiHandler() && server.isThereCgiHandler())
		{
			serverLocations[i].setCgiSettings(
				server.getCgiExtension(), server.getCgiInterpreter());
		}
		const std::map<std::string, std::string>
			&errorPages = server.getErrorPages();
		for (
			std::map<std::string, std::string>::const_iterator
				entry = errorPages.begin();
			entry != errorPages.end();
			++entry
		)
		{
			std::string code = entry->first;
			std::string url = entry->second;
			if (!serverLocations[i].isthereErrorPage(code))
				serverLocations[i].addErrorPage(code, url);
		}
	}
}

bool Parsing::isHTTPMethodValid(const std::string &method) const
{
	if (method == "GET") return true;
	if (method == "POST") return true;
	if (method == "DELETE") return true;
	return false;
}

void Parsing::setLocationLimitExcept(
	LocationConfig &location,
	const std::vector<std::string> &methods)
{
	std::map<std::string, bool> isMethodAllowed;
	isMethodAllowed["GET"] = false;
	isMethodAllowed["POST"] = false;
	isMethodAllowed["DELETE"] = false;
	for (size_t i = 0; i < methods.size(); ++i)
	{
		if (!isHTTPMethodValid(methods[i]))
		{
			std::cerr << "HTTP " << methods[i] << " is not implemented\n";
			throw ParseError();
		}
		isMethodAllowed[methods[i]] = true;
	}
	if (!isMethodAllowed["GET"]) location.addUnallowedMethod("GET");
	if (!isMethodAllowed["POST"]) location.addUnallowedMethod("POST");
	if (!isMethodAllowed["DELETE"]) location.addUnallowedMethod("DELETE");
}

void Parsing::setServerConfigListen(
	ServerConfig &server, 
	const std::string &token)
{
	int pos = token.size();
	while (--pos) {if (token[pos] == ':') break;}
	std::string ip = token.substr(0, pos);
	std::string port = token.substr(pos + 1);
	if (ip.empty() || port.empty() || !isPortValid(port))
	{
		std::cerr << "invalid ip[" << ip << "] port[" << port << "]\n";
		throw ParseError();
	}
	server.setHost(ip);
	server.setPort(port);
}

bool Parsing::isClientMaxBodySizeValid(const std::string &sizeStr) const
{
    std::stringstream sstream(sizeStr);
    size_t size;
    sstream >> size;
    std::ostringstream osstream;
    osstream << size;
    if (osstream.str() == sizeStr)
        return true;
    return false;
}
// multiple error pages
