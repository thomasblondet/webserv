#ifndef EVENTMANAGER_HPP
# define EVENTMANAGER_HPP
# include "ISelectable.hpp"
# include <sys/poll.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <signal.h>
# include <unistd.h>
# include <iostream>
# include <vector>
# include <map>

class EventManager
{
    public:
        ~EventManager();
        void add(ISelectable *obj, short events);
        void remove(int fd);
        void run();
        void removeClosedClients();

    private:
        std::vector<struct pollfd> fds;
        std::map<int, ISelectable*> objs;
};


#endif