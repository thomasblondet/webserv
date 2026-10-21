#include "EventManager.hpp"
#include <stdlib.h>

static int signalVar = 1;

void signalHandler(int tmp)
{
    static_cast<void>(tmp);
    signalVar = 0;
    std::cout << "\n* Server closed with ^C\n";
}

EventManager::~EventManager()
{
    // Free objects
    std::vector<std::pair<int, ISelectable*> > v;
    std::map<int, ISelectable*>::iterator it;
    for (it = objs.begin(); it != objs.end(); ++it)
        v.push_back(std::make_pair(it->first, it->second));

    for (int i = static_cast<int>(v.size()) - 1; i >=0; --i)
    {
        close(v[i].first);
        delete v[i].second;
    }
}

void EventManager::run()
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGQUIT, signalHandler);

    while (signalVar != 0 && !objs.empty())
    {
        poll(&fds[0], fds.size(), -1);
        for (int i = static_cast<int>(fds.size()) - 1; i >= 0; --i)
        {
            if (fds[i].revents & POLLIN)
            {
                std::map<int, ISelectable*>::iterator it = objs.find(fds[i].fd);
                if (it != objs.end())
                    it->second->onRead();
            }
            if (fds[i].revents & POLLOUT)
            {
                std::map<int, ISelectable*>::iterator it = objs.find(fds[i].fd);
                if (it != objs.end())
                    it->second->onWrite();
            }
        }
        removeClosedClients();
    }
}

void EventManager::add(ISelectable *obj, short events)
{
    struct pollfd add;

    add.fd = obj->getFD();
    add.events = events;
    add.revents = 0;
    fds.push_back(add);
    objs[obj->getFD()] = obj;
}

void EventManager::remove(int fd)
{
    std::map<int, ISelectable*>::iterator it_obj;

    it_obj = objs.find(fd);
    if (it_obj != objs.end())
    {
        ISelectable *tmp = it_obj->second;
        objs.erase(it_obj);
        delete tmp;
    }

    for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); ++it)
    {
        if (it->fd == fd)
        {
            fds.erase(it);
            break;
        }
    }
}

void EventManager::removeClosedClients()
{
    std::vector<int> toRemove;

    for (std::map<int, ISelectable*>::iterator it = objs.begin();
        it != objs.end(); ++it)
    {
        if (it->second->isClosed())
            toRemove.push_back(it->first);
    }

    for (size_t i = 0; i < toRemove.size(); ++i)
        remove(toRemove[i]);
}