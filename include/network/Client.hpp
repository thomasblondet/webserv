#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "ISelectable.hpp"
# include "EventManager.hpp"
# include "ServerConfig.hpp"
# include "HTTPParser.hpp"
# include "HTTPResponse.hpp"
# include "Router.hpp"
# include "IRequestHandler.hpp"
# include <string>
# include <cstring>

class Client : public ISelectable
{
	public:
		Client(int fd, EventManager *m, ServerConfig *c);
		Client(int fd, EventManager *m, ServerConfig *c, std::string ip, std::string port);
		~Client();
		int getFD() const;
		const std::string getType() const;
		void onRead();
		void onWrite();
		bool isClosed() const;
		void setIP(std::string);
		void setPort(std::string);
		std::string getIP() const;
		std::string getPort() const;
	
	private:
		int fd;
		EventManager *manager;
		ServerConfig *config;
		HTTPParser *parser;
		bool closed;
		// std::string writeBuffer;
		std::size_t writeOffset;
		std::string response;
		IRequestHandler *handler;
		std::string	ipStr;
		std::string	portStr;

		void loggingMsgOutgoingResponse() const;
		void loggingMsgIncomingRequest(const HTTPRequest&) const;
};

#endif