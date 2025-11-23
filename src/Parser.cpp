#include "Parser.hpp"

Parser::Parser(const std::string &config_file) : _config_file(config_file)
{
    init_handler();
}

void Parser::init_handler()
{
    handler["listen"] = &Parser::listen;
    handler["root"] = &Parser::root;
    handler["index"] = &Parser::index;
}

void trim_spaces(std::string &s)
{
    std::string::size_type first = 0;
    while (first < s.size() && std::isspace(static_cast<unsigned char>(s[first])))
        ++first;
    if (first == s.size())
	{
        s.clear();
        return;
    }
    std::string::size_type last = s.size() - 1;
    while (last > first && std::isspace(static_cast<unsigned char>(s[last])))
        --last;
    s = s.substr(first, last - first + 1);
}

std::vector<Config> Parser::build_config()
{
    std::vector<Config> v;
    std::ifstream ifs(_config_file.c_str());
    std::string line;

    while (std::getline(ifs, line))
	{
        std::istringstream iss(line);
        std::string token;
        iss >> token;
        if (line.empty() || line[0] == '#')
            continue;
        if (token == "server")
		{
            Config cfg;
            while (std::getline(ifs, line))
			{
                std::istringstream iss2(line);
                std::string token2;
                iss2 >> token2;
                trim_spaces(line);
                if (line.empty() || line[0] == '#')
                    continue;
                if (line[0] == '}')
                    break;
                // Call handler function
                std::map<std::string, Handler>::iterator it;
                it = handler.find(token2);
                if (it != handler.end())
				{
                    (this->*it->second)(cfg, line);
                    continue;
                }
            }
            v.push_back(cfg);
        }
    }
    return v;
}

void Parser::listen(Config& cfg, const std::string& line)
{
	std::istringstream iss(line);
	std::string token;
	int port;
	size_t octet = 0;

	std::getline(iss, token, ' ');

	// Parse address
	std::string address;
	std::getline(iss, address, ':');
	std::istringstream parse_address(address);
	std::string unit;
	while (std::getline(parse_address, unit, '.'))
	{
		std::istringstream convert(unit);
		int nbr;
		convert >> nbr;
		if (convert.fail() || !convert.eof() || nbr < 0 || nbr > 255)
			throw std::runtime_error("Invalid address");
		++octet;
	}
	if (octet != 4)
		throw std::runtime_error("Invalid address");

	// Parse port
	std::getline(iss, token, ' ');
	for (size_t i = 0; i < token.size(); ++i)
		if (!std::isdigit(token[i]))
			throw std::runtime_error("Invalid port");
	iss.clear();
	iss.str(token);
	iss >> port;
	if (iss.fail() || !iss.eof() || port < 1 || port > 65535)
		throw std::runtime_error("Invalid port");
	
	cfg.address = address;
	cfg.port = token;
}

void Parser::root(Config& cfg, const std::string& line)
{
	struct stat info;
	std::istringstream iss(line);
	std::string token;

	iss >> token; iss >> token;

	if (stat(token.c_str(), &info) || iss.fail() || !iss.eof())
		throw std::runtime_error("Invalid root");
	
	cfg.root = token;
}

void Parser::index(Config& cfg, const std::string& line)
{
	std::istringstream iss(line);
	std::string token;

	iss >> token; iss >> token;

	std::string path = cfg.root + "/" + token;
	std::ifstream ifs(path.c_str());
	if (!ifs.is_open())
		throw std::runtime_error("Invalid index");

	cfg.index = token;
}
