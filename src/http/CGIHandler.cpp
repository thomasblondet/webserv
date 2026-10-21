#include "CGIHandler.hpp"
#include "HTTPResponse.hpp"
#include "Helpers.hpp"
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>

CGIHandler::CGIHandler(
	const std::string &path, 
	HTTPRequest &request,
	LocationConfig &location,
	EventManager &manager):
	_path(path),
	_request(request),
	_location(location),
	_manager(manager),
	_isResponseReady(false),
	_inputListener(request),
	_outputListener(*this),
	_p_outputListener(NULL)
{
	(void) _location;
}

CGIHandler::~CGIHandler()
{
	if (_p_outputListener)
		_p_outputListener->setCGIHandler(NULL);
}

HTTPResponse CGIHandler::handle()
{
	if (access(_path.c_str(), R_OK) || access(_path.c_str(), X_OK))
		returnErrorResponse(403);
	int server_to_cgi[2];
	int cgi_to_server[2];
	if (pipe(server_to_cgi) == -1)
		returnErrorResponse(500);
	if (pipe(cgi_to_server) == -1)
	{
		close(server_to_cgi[READ_END]);
		close(server_to_cgi[WRITE_END]);
		returnErrorResponse(500);
	}
	pid_t pid = fork();
	if (pid == -1)
	{
		close(server_to_cgi[READ_END]);
		close(server_to_cgi[WRITE_END]);
		close(cgi_to_server[READ_END]);
		close(cgi_to_server[WRITE_END]);
		returnErrorResponse(500);
	}
	_cgiStart = std::time(NULL);
	if (pid == 0)
		launchCgiProcess(server_to_cgi, cgi_to_server);
	else
	{
		_cpid = pid;
		close(server_to_cgi[READ_END]);
		close(cgi_to_server[WRITE_END]);
		_inputListener.setFD(server_to_cgi[WRITE_END]);
		_outputListener.setFD(cgi_to_server[READ_END]);
		_manager.add(new CgiInputListener(_inputListener), POLLOUT);
		_p_outputListener = new CgiOutputListener(_outputListener);
		_manager.add(_p_outputListener, POLLIN);
	}
	return _response;
}

HTTPResponse CGIHandler::getResponse() const
{
	return _response;
}

std::string	CGIHandler::getResponseStr() const
{
	return _responseStr;
}

void CGIHandler::setResponseStr(std::string &response)
{
	_responseStr = response;
}

bool CGIHandler::isResponseReady()
{
	if (_isResponseReady)
		return true;
	int status;
	pid_t ret = waitpid(_cpid, &status, WNOHANG);
	if (ret > 0)
	{
		if (WIFEXITED(status))
		{
			int code = WEXITSTATUS(status);
			std::cout << "CGI exited with code " << code << "\n";
			if (code == EXIT_FAILURE)
				returnErrorResponse(500);
		}
		else if (WIFSIGNALED(status))
		{
			std::cout << "CGI killed by signal " << WTERMSIG(status) << "\n";
			returnErrorResponse(500);
		}
		setResponseReady(true);
		return true;
	}
	else if (ret == 0)
	{
		std::time_t now = std::time(NULL);
		if (now >= _cgiStart + CGI_TIMEOUT)
		{
			kill(_cpid, SIGKILL);
			waitpid(_cpid, NULL, 0);
			returnErrorResponse(504);
			setResponseReady(true);
			return true;
		}
		return false;
	}
	else
	{
		returnErrorResponse(500);
		setResponseReady(true);
		return true;
	}
}

void CGIHandler::setResponseReady(bool isReady)
{
	_isResponseReady = isReady;
}

void CGIHandler::launchCgiProcess(int server_to_cgi[2], int cgi_to_server[2])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);

	// Close unused ends
	close(server_to_cgi[WRITE_END]);
	close(cgi_to_server[READ_END]);

	// Pipe → stdin
	if (dup2(server_to_cgi[READ_END], STDIN_FILENO) == -1)
	{
		perror("dup2 stdin");
		exit(EXIT_FAILURE);
	}

	// Pipe → stdout
	if (dup2(cgi_to_server[WRITE_END], STDOUT_FILENO) == -1)
	{
		perror("dup2 stdout");
		exit(EXIT_FAILURE);
	}

	// We can close original fds after duplication
	close(server_to_cgi[READ_END]);
	close(cgi_to_server[WRITE_END]);

	char *const argv[] = { const_cast<char*>(_path.c_str()), NULL };
	char *const *envp = getEnvpFromRequest();

	execve(_path.c_str(), argv, envp);

	std::cerr << "Unable to launch CGI process\n";
	if (envp)
	{
		size_t i = 0;
		while (envp[i]) {delete envp[i]; ++i;}
	}
	delete envp;

	exit(EXIT_FAILURE);
}

char *const *CGIHandler::getEnvpFromRequest()
{
	std::vector<std::string> envv;
	std::map<std::string, std::string> headers = _request.getHeaders();
    std::map<std::string, std::string>::iterator it;
	envv.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envv.push_back("SERVER_SOFTWARE=Webserv/1.0");
	envv.push_back("REQUEST_METHOD=" + _request.getMethod());
	envv.push_back("QUERY_STRING=" + _request.getQuery());
	envv.push_back("SERVER_PROTOCOL=" + _request.getHTTPVersion());
	envv.push_back("SCRIPT_NAME=" + _request.getPath());
	envv.push_back("SCRIPT_FILENAME=" + _path);
	envv.push_back("SERVER_NAME=" + _request.getServertIP());
	envv.push_back("SERVER_PORT=" + _request.getServertPort());
	envv.push_back("REMOTE_ADDR=" + _request.getClientIP());
	// headers
    for (it = headers.begin(); it != headers.end(); ++it)
	{
		std::string var;
		for (size_t i=0; i<it->first.size(); ++i)
		{
			if (it->first[i] == '-') var += "_";
			else var += toupper(it->first[i]);
		}
		if (var != "CONTENT_LENGTH" && var != "CONTENT_TYPE") var = "HTTP_" + var;
		envv.push_back(var + "=" + it->second);
	}
	char **envp = new (std::nothrow) char*[envv.size() + 1];
	if (!envp)
		return NULL;
	for (size_t i = 0; i < envv.size(); ++i)
	{
		envp[i] = new (std::nothrow) char[envv[i].size() + 1];
		if (!envp[i])
			return NULL;
		std::strcpy(envp[i], envv[i].c_str());
	}
	envp[envv.size()] = NULL;
	return envp;
}

HTTPResponse CGIHandler::returnErrorResponse(int statusCode)
{
	setResponseReady(true);
	_response = Helpers::error(statusCode);
	_responseStr = _response.buildResponse();
	return _response;
}

void CGIHandler::setCgiOutputListener(CgiOutputListener	*p_outputListener)
{
	_p_outputListener = p_outputListener;
}

/*******************************************************************************
	CgiInputListener
*******************************************************************************/
CgiInputListener::CgiInputListener(HTTPRequest	&request):
	_fd(-1), 
	_isClosed(false),
	_request(request)
{}

int CgiInputListener::getFD() const
{
	return _fd;
}

void CgiInputListener::setFD(int fd)
{
	_fd = fd;
}

const std::string CgiInputListener::getType() const
{
	return "CgiInputListener";
}

void CgiInputListener::onRead() {};

void CgiInputListener::onWrite()
{
	if (_isClosed)
		return;
	if (_request.getBody().size())
	{
		ssize_t n = write(_fd, _request.getBody().c_str(), _request.getBody().size());
		if (n <= 0)
			std::cerr << "Unable to write to CGI.\n";
	}
	close(_fd);
	_isClosed = true;
}

bool CgiInputListener::isClosed() const
{
	return _isClosed;
}

/*******************************************************************************
	CgiOutputListener
*******************************************************************************/
CgiOutputListener::CgiOutputListener(CGIHandler &cgiHandler):
	_fd(-1),
	_isClosed(false),
	_cgihandler(&cgiHandler)
{}

CgiOutputListener::~CgiOutputListener()
{
	if (_cgihandler)
		_cgihandler->setCgiOutputListener(NULL);
}

int CgiOutputListener::getFD() const
{
	return _fd;
}

void CgiOutputListener::setFD(int fd)
{
	_fd = fd;
}

const std::string CgiOutputListener::getType() const
{
	return "CgiOutputListener";
}

void CgiOutputListener::onRead()
{
	unsigned char temp[1024];
	ssize_t n = read(_fd, temp, sizeof(temp));
	_buffer.insert(_buffer.end(), temp, temp + n);
	if (n < 0)
	{
		std::cerr << "Unable to read from CGI.\n";
		close(_fd);
		_isClosed = true;
		return;
	}
	if (n < 1024)
	{
		for (std::size_t i = 0; i < _buffer.size(); ++i)
			_responseStr += _buffer[i];
		close(_fd);
		_isClosed = true;
		formatCgiResponse();
		if (_cgihandler)
		{
			_cgihandler->setResponseStr(_responseStr);
			_cgihandler->setResponseReady(true);
		}
	}
}

void CgiOutputListener::onWrite() {};

bool CgiOutputListener::isClosed() const
{
	return _isClosed;
}

std::string	CgiOutputListener::getResponseStr() const
{
	return _responseStr;
}

void CgiOutputListener::formatCgiResponse()
{
    size_t pos = _responseStr.find("\r\n\r\n");
    size_t sep_len = 4;

    if (pos == std::string::npos)
    {
        pos = _responseStr.find("\n\n");
        sep_len = 2;
    }

    std::string statusLine = StatusLine::get(200) + "\r\n";
    std::string headers;
    std::string body;

    // no headers, only body
    if (pos == std::string::npos)
    {
        body = _responseStr;

        headers += "Content-Type: application/octet-stream\r\n";
        headers += "Connection: close\r\n";
        headers += "Content-Length: " + intToString(body.size()) + "\r\n";
        headers += "\r\n";

        _responseStr = statusLine + headers + body;
        return;
    }

    // headers + body
    std::string cgi_headers = _responseStr.substr(0, pos);
    body = _responseStr.substr(pos + sep_len);

    std::istringstream stream(cgi_headers);
    std::string line;
    bool hasContentType = false;

    while (std::getline(stream, line))
    {
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);

        if (line.empty())
            continue;

        if (line.compare(0, 7, "Status:") == 0)
        {
            std::string status = line.substr(7);
            while (!status.empty() && status[0] == ' ')
                status.erase(0, 1);

            statusLine = "HTTP/1.1 " + status + "\r\n";
            continue;
        }

        if (line.compare(0, 13, "Content-Type:") == 0)
            hasContentType = true;

        headers += line + "\r\n";
    }

    if (!hasContentType)
        headers += "Content-Type: application/octet-stream\r\n";

    headers += "Connection: close\r\n";
    headers += "Content-Length: " + intToString(body.size()) + "\r\n";
    headers += "\r\n";

    _responseStr = statusLine + headers + body;
}

std::string CgiOutputListener::intToString(size_t n)
{
    std::ostringstream oss;
    oss << n;
    return oss.str();
}

void CgiOutputListener::setCGIHandler(CGIHandler *cgihandler)
{
	_cgihandler = cgihandler;
}
