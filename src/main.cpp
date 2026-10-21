#include "parsing.hpp"
#include "Server.hpp"
#include <iostream>

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: " << av[0] << " <configuration.conf>" << std::endl;
		return 1;
	}

	try
	{
		Parsing	parser;
		parser.parsing(av[1]);
		std::vector<ServerConfig> configs = parser.getConfig();
		Server server(configs);
	}
	catch (const Parsing::ParseError &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}