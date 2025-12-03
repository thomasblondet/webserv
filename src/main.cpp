#include "Server.hpp"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./webserv <configuration file>\n";
		return 1;
	}
	
	(void)argv; // temp
	try
	{
		// Test with hardcoded Config file for now
		Config cfg = { "127.0.0.1", "8081", "www", "index.html" };
		std::vector<Config> listeners;
		listeners.push_back(cfg);

		// Start the server with the Config file
		Server s(listeners);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return 1;
	}
	return 0;
}
