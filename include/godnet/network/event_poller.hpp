#ifndef GODNET_NETWORK_EVENT_POLLER_HPP
#define GODNET_NETWORK_EVENT_POLLER_HPP

#include "godnet/config.hpp"
#include "godnet/util/noncopyable.hpp"

#include <cstdint>
#include <vector>
#include <unordered_set>

struct epoll_event;

namespace godnet
{

class EventLoop;
class EventChannel;

class GODNET_EXPORT EventPoller : Noncopyable
{
public:
    explicit EventPoller(EventLoop* loop);
    ~EventPoller();

    void poll(std::vector<EventChannel*>& readyChannels, int timeout = -1);
    void update(EventChannel* channel);

private:
    void ctl(int op, EventChannel* channel);

    EventLoop* loop_;
#if defined(GODNET_LINUX)
    int epoll_fd_;
#elif defined(GODNET_WIN)
    void* epoll_fd_;
#endif
    std::vector<struct epoll_event> events_;
    std::unordered_set<EventChannel*> channels_;
};

}

#endif
