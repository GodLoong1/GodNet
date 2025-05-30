#ifndef GODNET_NET_EVENT_POLLER_HPP
#define GODNET_NET_EVENT_POLLER_HPP

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

class EventPoller : Noncopyable
{
public:
#ifdef _WIN32
    using EventCallback = std::function<void(std::uint64_t)>;
#endif

    explicit EventPoller(EventLoop* loop) noexcept;
    ~EventPoller();

    void pollEvents(std::vector<EventChannel*>& readyChannels,
                    int timeout) noexcept;
    void updateChannel(EventChannel* channel) noexcept;

#ifdef _WIN32
    void postEvent(std::uint64_t event);

    void setPostEvent(EventCallback&& callback) noexcept
    {
        eventCallback_ = std::move(callback);
    }
#endif

private:
    void ctl(int op, EventChannel* channel) noexcept;

    EventLoop* loop_;
#ifdef __linux__
    int epollFd_;
#else
    void* epollFd_;
    EventCallback eventCallback_;
#endif
    std::vector<struct epoll_event> events_;
    std::unordered_set<EventChannel*> channels_;
};

}

#endif
