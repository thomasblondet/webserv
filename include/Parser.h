#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <sys/stat.h>

typedef struct Config {
    std::string address;
    std::string port;
    std::string root;
    std::string index;
} Config;

class Parser {
public:
    Parser(const std::string& config_file);
    std::vector<Config> build_config();
private:
    std::string m_config_file;

    // Handler
    typedef void (Parser::*Handler)(Config&, const std::string&);
	std::map<std::string, Handler> handler;
	void init_handler();

	// Handler functions
	void listen(Config& cfg, const std::string& line);
	void root(Config& cfg, const std::string& line);
	void index(Config& cfg, const std::string& line);
};

#endif // PARSER_H
