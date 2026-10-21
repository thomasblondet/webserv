#ifndef LISTENER_HPP
# define LISTENER_HPP
# include "ISelectable.hpp"
# include "EventManager.hpp"
# include "ServerConfig.hpp"
# include "Client.hpp"
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <fcntl.h>
# include <cstring>

class Listener : public ISelectable
{
	public:
		Listener(EventManager *m, ServerConfig *c);
		~Listener();
		int getFD() const;
		const std::string getType() const;
		void onRead();
		void onWrite();
		bool isClosed() const;

	private:
		int fd;
		EventManager *manager;
		ServerConfig *config;
};

#endif
