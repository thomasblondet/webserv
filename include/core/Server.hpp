#ifndef SERVER_HPP
# define SERVER_HPP
# include "EventManager.hpp"
# include "ISelectable.hpp"
# include "Listener.hpp"
# include "ServerConfig.hpp"

class Server
{
	public:
		Server(ServerConfig *config);
		Server(std::vector<ServerConfig> configs);
		~Server();

	private:
		EventManager *manager;
};

#endif