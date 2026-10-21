#ifndef DELETEHANDLER_HPP
# define DELETEHANDLER_HPP
# include "IRequestHandler.hpp"
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "StatusLine.hpp"
# include "Helpers.hpp"
# include <stdio.h>
# include <sys/stat.h>
# include <iostream>
# include <fstream>
# include <sstream>
# include <exception>
# include <string>
# include <algorithm>

class DeleteHandler : public IRequestHandler
{
	public:
		DeleteHandler(const std::string &path);
		~DeleteHandler();
		HTTPResponse handle();

	private:
		std::string path;
};

#endif