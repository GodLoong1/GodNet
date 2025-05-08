#ifndef GODNET_NETWORK_EVENT_POLLER_HPP
#define GODNET_NETWORK_EVENT_POLLER_HPP

#include "godnet/config.hpp"
#include "godnet/util/noncopyable.hpp"

#include <vector>
#include <unordered_set>
#ifdef _WIN32
    #include <cstdint>
    #include <functional>
#endif

struct epoll_event;

namespace godnet
{

class EventLoop;
class EventChannel;

class GODNET_EXPORT EventPoller : Noncopyable
{
public:
#ifdef _WIN32
    using EventCallback = std::function<void(std::uint64_t)>;
#endif

    explicit EventPoller(EventLoop* loop);
    ~EventPoller();

    void pollEvents(std::vector<EventChannel*>& readyChannels, int timeout);
    void updateChannel(EventChannel* channel);
#ifdef _WIN32
    void postEvent();
#endif

private:
    void ctl(int op, EventChannel* channel);

    EventLoop* loop_;
#if defined(GODNET_LINUX)
    int epollFd_;
#elif defined(GODNET_WIN)
    void* epollFd_;
    EventCallback eventCallback_;
#endif
    std::vector<struct epoll_event> events_;
    std::unordered_set<EventChannel*> channels_;
};

}

#endif
