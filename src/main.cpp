#include "Server.h"

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: ./webserv <configuration file>\n";
		return 1;
	}
	Parser p((std::string(argv[1])));
	std::vector<Config> v = p.build_config();
	Server s(v);
	return 0;
}
