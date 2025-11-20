#include "Server.h"

Server::Server(const std::vector<Config>& v)
{
    for (size_t i = 0; i < v.size(); i++) {
        Config c = {
            .address = v[i].address,
            .port = v[i].port,
            .root = v[i].root,
            .index = v[i].index,
        };
        m_cfgs.push_back(c);
        struct pollfd listener = { init_listener(m_cfgs[i]), POLLIN, 0 };
        m_fds.push_back(listener);
    }
    accept_connection();
}

// tmp
void Server::print(std::vector<Config> v) const 
{
	for (size_t i = 0; i < v.size(); i++) {
		if (!v[i].address.empty())
			std::cout << v[i].address << std::endl;
		if (!v[i].port.empty())
			std::cout << v[i].port << std::endl;
		if (!v[i].root.empty())
			std::cout << v[i].root << std::endl;
		if (!v[i].index.empty())
			std::cout << v[i].index << std::endl;
		std::cout << std::endl;
	}
}

int Server::init_listener(Config cfg)
{
    int listening_socket;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	int yes = 1;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int rv = getaddrinfo(cfg.address.c_str(), cfg.port.c_str(), &hints, &servinfo);
	if (rv != 0)
		throw std::runtime_error("getaddrinfo");
	listening_socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (listening_socket == -1)
		throw std::runtime_error("socket");
	rv = setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (rv == -1)
		throw std::runtime_error("setsockopt");
	rv = bind(listening_socket, servinfo->ai_addr, servinfo->ai_addrlen);
	if (rv == -1)
		throw std::runtime_error("bind");

	freeaddrinfo(servinfo);

	rv = listen(listening_socket, 10);
	if (rv == -1)
		throw std::runtime_error("listen");

    return listening_socket;
}

void Server::accept_connection()
{
    std::cout << "Waiting for connection..." << std::endl;
    std::vector<int> listening_sockets;
    for (size_t i = 0; i < m_fds.size(); i++)
        listening_sockets.push_back(m_fds[i].fd);

    while (1) {
        // Pointer to pollfd vector
        struct pollfd* ppollfds = &m_fds[0];
        poll(ppollfds, m_fds.size(), -1);
        for (size_t i = 0; i < m_fds.size(); i++) {
            std::vector<int>::iterator it;
            it = std::find(listening_sockets.begin(), listening_sockets.end(), ppollfds[i].fd);
            if (it != listening_sockets.end() && ppollfds[i].revents & POLLIN) {
                struct sockaddr_storage client_addr;
				socklen_t addr_len = sizeof(client_addr);
                int fd = accept(m_fds[i].fd, (struct sockaddr *)&client_addr, &addr_len);
                struct pollfd new_client = {fd, POLLIN, 0};
                m_fds.push_back(new_client);
                std::cout << "New connection" << std::endl;
            }
            else if (ppollfds[i].revents & POLLIN) {
                char buf[1024];
				memset(buf, 0, 1024);
				ssize_t nrecv = recv(ppollfds[i].fd, buf, 1024, 0);
				if (nrecv <= 0) {
					close(ppollfds[i].fd);
					m_fds.erase(m_fds.begin() + 1);
					i--;
				}
				else {
					std::string data(buf);
					Request request;
                    request = parse_request(buf);
                    // ...
                }
            }
        }
    }
}

// En travaux
Request Server::parse_request(const std::string& buf)
{
    Request req;
    std::istringstream iss(buf);
    std::string line, token;

    std::getline(iss, token, ' ');
    req.method = token;
    std::getline(iss, token, ' ');
    req.uri = token;
    std::getline(iss, token, '\r');
    req.version= token;

    while (std::getline(iss, line)) {
        std::istringstream iss2(line);
        std::string keyword;
        iss >> keyword;
        keyword = keyword.substr(0, keyword.size() - 1);
        iss >> token;
        req.headers[keyword] = token;
    }
    
    std::cout << req.headers["host"] << std::endl;
    // TODO optional body
    return req;
}
