#include "godnet/net/event_poller.hpp"

#ifdef _WIN32
    #include "wepoll.h"
#else
    #include <unistd.h>
    #include <sys/epoll.h>
#endif

#include <cassert>

#include "godnet/util/logger.hpp"
#include "godnet/net/event_loop.hpp"
#include "godnet/net/event_channel.hpp"

namespace godnet
{

EventPoller::EventPoller(EventLoop* loop) noexcept
: loop_(loop),
  events_(32)
{
#ifdef __linux__
    epollFd_ = ::epoll_create1(EPOLL_CLOEXEC);
    assert(epollFd_ >= 0);
#else
    epollFd_ = ::epoll_create1(0);
    assert(epollFd_);
#endif
}

EventPoller::~EventPoller()
{
#ifdef __linux__
    ::close(epollFd_);
#else
    ::epoll_close(epollFd_);
#endif
}

void EventPoller::pollEvents(std::vector<EventChannel*>& readyChannels,
                             int timeout) noexcept
{
    loop_->assertInLoopThread();

    int ready = ::epoll_wait(epollFd_,
                             events_.data(),
                             static_cast<int>(events_.size()),
                             timeout);
    if (ready > 0)
    {
        if (ready == static_cast<int>(events_.size()))
        {
            events_.resize(events_.size() * 2);
        }

        for (std::size_t i = 0; i != static_cast<std::size_t>(ready); ++i)
        {
            auto& ev = events_[i];
#ifdef _WIN32
            if (ev.events & EPOLLEVENT)
            {
                eventCallback_(events_[i].data.u64);
                continue;
            }
#endif
            auto* channel = static_cast<EventChannel*>(ev.data.ptr);

            channel->setRevents(ev.events);
            readyChannels.emplace_back(channel);
        }
    }
    else if (ready < 0)
    {
        GODNET_LOG_SYSERR("epoll_wait loopThreadId: {}", loop_->threadId());
    }
}

void EventPoller::updateChannel(EventChannel* channel) noexcept
{
    loop_->assertInLoopThread();

    if (channels_.find(channel) != channels_.end())
    {
        if (channel->isNoneEvent())
        {
            ctl(EPOLL_CTL_DEL, channel);

            [[maybe_unused]] auto n =  channels_.erase(channel);
            assert(n == 1);
        }
        else
        {
            ctl(EPOLL_CTL_MOD, channel);
        }
    }
    else
    {
        ctl(EPOLL_CTL_ADD, channel);

        [[maybe_unused]] auto it = channels_.insert(channel);
        assert(it.second);
    }
}

#ifdef _WIN32
void EventPoller::postEvent(std::uint64_t event) noexcept
{
    loop_->assertInLoopThread();

    ::epoll_post_event(epollFd_, event);
}
#endif

void EventPoller::ctl(int op, EventChannel* channel) noexcept
{
    struct epoll_event ev{};
    ev.data.ptr = channel;
    ev.events = channel->events();

    if (::epoll_ctl(epollFd_, op, channel->fd(), &ev) < 0)
    {
        GODNET_LOG_SYSERR("epoll_ctl loopThreadId: {}, channelFd: {}",
            loop_->threadId(),
            channel->fd());
    }
}

}