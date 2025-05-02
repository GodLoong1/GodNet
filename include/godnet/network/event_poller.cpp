#include "godnet/network/event_poller.hpp"

#include "godnet/util/debug.hpp"
#include "godnet/network/event_loop.hpp"
#include "godnet/network/event_channel.hpp"

#if defined(GODNET_WIN)
    #include "wepoll.h"
#elif defined(GODNET_LINUX)
    #include <unistd.h>
    #include <sys/epoll.h>
#endif

namespace godnet
{

EventPoller::EventPoller(EventLoop* loop)
: loop_(loop),
  events_(32)
{
    epoll_fd_ = ::epoll_create(1);
    if (epoll_fd_ < 0)
    {
        GODNET_THROW_RUNERR("Failed to create epoll file descriptor");
    } 
}

EventPoller::~EventPoller()
{
    if (epoll_fd_ >= 0)
    {
        ::close(epoll_fd_);
    }
}

void EventPoller::pollEvents(std::vector<EventChannel*>& readyChannels, int timeout)
{
    loop_->assertInLoopThread();

    int ready = ::epoll_wait(epoll_fd_, events_.data(), static_cast<int>(events_.size()), timeout);
    if (ready > 0)
    {
        if (ready == static_cast<int>(events_.size()))
        {
            events_.resize(events_.size() * 2);
        }

        for (std::size_t i = 0; i != static_cast<std::size_t>(ready); ++i)
        {
            auto& ev = events_[i];
            auto* channel = static_cast<EventChannel*>(ev.data.ptr);

            channel->setRevents(ev.events);
            readyChannels.emplace_back(channel);
        }
    }
    else
    {
        GODNET_THROW_SYSERR("epoll_wait error");
    }
}

void EventPoller::updateChannel(EventChannel* channel)
{
    loop_->assertInLoopThread();

    if (channels_.find(channel) != channels_.end())
    {
        if (channel->isNoneEvent())
        {
            ctl(EPOLL_CTL_DEL, channel);
            channels_.erase(channel);
        }
        else
        {
            ctl(EPOLL_CTL_MOD, channel);
            channels_.insert(channel);
        }
    }
    else
    {
        ctl(EPOLL_CTL_ADD, channel);
    }
}

void EventPoller::ctl(int op, EventChannel* channel)
{
    struct epoll_event ev;
    ev.data.ptr = channel;
    ev.events = channel->events();

    ::epoll_ctl(epoll_fd_, op, channel->fd(), &ev);
}

}
