#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "IRequestHandler.hpp"
# include "EventManager.hpp"
#include <sys/types.h>
#include <unistd.h>
#include <ctime>

# define READ_END 0
# define WRITE_END 1
# define CGI_TIMEOUT 5 // seconds

class CGIHandler;

/*
	CgiInputListener: should be in his own file.
*/
class CgiInputListener: public ISelectable
{
	public:
		CgiInputListener(HTTPRequest &);

		const std::string getType() const;
		int		getFD() const;
		void	setFD(int fd);
		void	onRead();
		void	onWrite();
		bool	isClosed() const;

	private:
		int			_fd;
		bool		_isClosed;
		HTTPRequest	_request;
};

/*
	CgiOutputListener: should be in his own file.
*/
class CgiOutputListener: public ISelectable
{
	public:
		CgiOutputListener(CGIHandler &);
		~CgiOutputListener();

		const std::string getType() const;
		int		getFD() const;
		void	setFD(int fd);
		void	onRead();
		void	onWrite();
		bool	isClosed() const;
		std::string		getResponseStr() const;
		void			setCGIHandler(CGIHandler *cgihandler);

	private:
		int			_fd;
		bool		_isClosed;
		std::string	_responseStr;
		CGIHandler	*_cgihandler;
		std::vector<unsigned char> _buffer;

		void		formatCgiResponse();
		std::string intToString(size_t n);
};

class CGIHandler : public IRequestHandler
{
	public:
		CGIHandler(const std::string &, HTTPRequest &, LocationConfig &, EventManager &);
		~CGIHandler();

		HTTPResponse	handle();
		HTTPResponse	getResponse() const;
		void			setResponseStr(std::string &);
		std::string		getResponseStr() const;
		bool			isResponseReady();
		void			setResponseReady(bool isReady);
		void			setCgiOutputListener(CgiOutputListener *_p_outputListener);

	private:
		const std::string	_path;
		HTTPRequest			&_request;
		LocationConfig		&_location;
		EventManager		&_manager;
		bool				_isResponseReady;
		HTTPResponse		_response;
		std::string			_responseStr;
		CgiInputListener	_inputListener;
		CgiOutputListener	_outputListener;
		CgiOutputListener	*_p_outputListener;
		pid_t				_cpid;
    	std::time_t 		_cgiStart;

		void				launchCgiProcess(int server_to_cgi[2], int cgi_to_server[2]);
		char *const * 		getEnvpFromRequest();
		HTTPResponse		returnErrorResponse(int statusCode);

};

#endif // CGIHANDLER_HPP