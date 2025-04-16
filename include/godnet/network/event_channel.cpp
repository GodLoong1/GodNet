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

const std::uint32_t EventChannel::NONE_EVENT = 0;
const std::uint32_t EventChannel::READ_EVENT = EPOLLIN | EPOLLPRI;
const std::uint32_t EventChannel::WRITE_EVENT = EPOLLOUT;

EventChannel::EventChannel(EventLoop* loop, int fd)
: loop_(loop),
  fd_(fd)
{
    GODNET_THROW_IF(!loop, "EventLoop is null");
    GODNET_THROW_IF(fd < 0, "Invalid file descriptor");
}

EventChannel::~EventChannel()
{
    GODNET_ASSERT(!is_handling_ && "EventChannel is handling events");
}

void EventChannel::update()
{
    loop_->assertInLoop();
    loop_->updateChannel(this);
}

void EventChannel::handlerEvent()
{
    loop_->assertInLoop();

    if (isNoneEvent())
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
    if (revents_ & EPOLLOUT && !(revents_ & EPOLLHUP))
    {
        if (write_callback_)
        {
            write_callback_();
        }
    }
    is_handling_ = false;
}

}
