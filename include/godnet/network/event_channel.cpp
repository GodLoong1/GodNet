#include "godnet/network/event_channel.hpp"

#include "godnet/util/debug.hpp"
#include "godnet/network/event_loop.hpp"

#include <cassert>

#if defined(GODNET_WIN)
    #include "wepoll.h"
#elif defined(GODNET_LINUX)
    #include <sys/epoll.h>
#endif

namespace godnet
{

EventChannel::EventChannel(EventLoop* loop, int fd)
: loop_(loop),
  fd_(fd)
{
    if (!loop)
    {
        GODNET_THROW("EventLoop is null");
    }
    if (fd < 0)
    {
        GODNET_THROW("Invalid file descriptor");
    }
}

EventChannel::~EventChannel()
{
    GODNET_ASSERT(!is_handling_ && "EventChannel is handling events");
}

void EventChannel::enableReading()
{
    events_ |= EPOLLIN;
    update();
}

void EventChannel::disableReading()
{
    events_ &= ~EPOLLIN;
    update();
}

bool EventChannel::isReading() const noexcept
{
    return events_ & EPOLLIN;
}

void EventChannel::enableWriting()
{
    events_ |= (EPOLLOUT | EPOLLPRI);
    update();
}

void EventChannel::disableWriting()
{
    events_ &= ~(EPOLLOUT | EPOLLPRI);
    update();
}

bool EventChannel::isWriting() const noexcept
{
    return events_ & (EPOLLOUT | EPOLLPRI);
}

bool EventChannel::isNone() const noexcept
{
    return events_ == 0;
}

void EventChannel::disableAll()
{
    events_ = 0;
    update();
}

void EventChannel::update()
{
    loop_->updateChannel(this);
}

void EventChannel::handlerEvent()
{
    if (events_ == 0)
    {
        return;
    }
    if (is_bind)
    {
        if (auto object = object_handler_.lock())
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
    if (event_callback_)
    {
        event_callback_();
        return;
    }
    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
    {
        if (close_callback_)
        {
            close_callback_();
        }
    }
    if (revents_ & EPOLLERR)
    {
        if (error_callback_)
        {
            error_callback_();
        }
    }
    if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if (read_callback_)
        {
            read_callback_();
        }
    }
    if (revents_ & EPOLLOUT)
    {
        if (write_callback_)
        {
            write_callback_();
        }
    }
    is_handling_ = false;
}

}
