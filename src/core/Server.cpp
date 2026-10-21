#include "Server.hpp"

Server::Server(ServerConfig *config) : manager(new EventManager)
{
	std::cout << "* Server Started" << std::endl;

	ISelectable *l = new Listener(manager, config);
	manager->add(l, POLLIN);
	manager->run();
}

Server::Server(std::vector<ServerConfig> configs): 
	manager(new EventManager)
{
	std::cout << "* Server Started" << std::endl;

	for (size_t i = 0; i < configs.size(); ++i)
	{
		try
		{
			ISelectable *l = new Listener(manager, &configs[i]);
			manager->add(l, POLLIN);
		}
		catch (std::runtime_error &e)
		{
			std::cerr << "Fail to run server [" << i << "]: " << 
			configs[i].getHost() << ":" << configs[i].getPort() << "\n";
			return;
		}
	}
	manager->run();
}

Server::~Server()
{
	std::cout << "* Server Stopped" << std::endl;
	delete manager;
}