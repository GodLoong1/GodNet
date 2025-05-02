#include "godnet/network/event_channel.hpp"

#if defined(GODNET_WIN)
    #include "wepoll.h"
#elif defined(GODNET_LINUX)
    #include <sys/epoll.h>
#endif

#include "godnet/util/debug.hpp"
#include "godnet/network/event_loop.hpp"


namespace godnet
{

const std::uint32_t EventChannel::NONE_EVENT = 0;
const std::uint32_t EventChannel::READ_EVENT = EPOLLIN | EPOLLPRI;
const std::uint32_t EventChannel::WRITE_EVENT = EPOLLOUT;

EventChannel::EventChannel(EventLoop* loop, int fd)
: loop_(loop), fd_(fd)
{
    if (!loop)
    {
        GODNET_THROW_RUNERR("EventLoop is nullptr");
    }
    if (fd < 0)
    {
        GODNET_THROW_RUNERR("fd is invalid");
    }
}

EventChannel::~EventChannel()
{
    GODNET_ASSERT(!is_handling_ && "EventChannel is handling events");
}

void EventChannel::updateChannel()
{
    loop_->assertInLoopThread();

    loop_->updateChannel(this);
}

void EventChannel::handlerEvent()
{
    loop_->assertInLoopThread();

    if (isNoneEvent())
    {
        return;
    }
    if (is_bind)
    {
        if (auto object = bindObject_.lock())
        {
            handlerEventSafe(); 
        }
    }
    else
    {
        handlerEventSafe();
    }
}

void EventChannel::handlerEventSafe()
{
    is_handling_ = true;
    if (eventCallback_)
    {
        eventCallback_();
        return;
    }
    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
    {
        if (closeCallback_)
        {
            closeCallback_();
        }
    }
    if (revents_ & EPOLLERR)
    {
        if (errorCallback_)
        {
            errorCallback_();
        }
    }
    if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if (readCallback_)
        {
            readCallback_();
        }
    }
    if (revents_ & EPOLLOUT && !(revents_ & EPOLLHUP))
    {
        if (writeCallback_)
        {
            writeCallback_();
        }
    }
    is_handling_ = false;
}

}