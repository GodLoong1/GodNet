#include "godnet/net/event_channel.hpp"

#ifdef _WIN32
    #include "wepoll.h"
#else
    #include <sys/epoll.h>
#endif

#include <cassert>

#include "godnet/net/event_loop.hpp"

namespace godnet
{

const std::uint32_t EventChannel::NONE_EVENT = 0;
const std::uint32_t EventChannel::READ_EVENT = EPOLLIN | EPOLLPRI;
const std::uint32_t EventChannel::WRITE_EVENT = EPOLLOUT;

EventChannel::EventChannel(EventLoop* loop, int fd) noexcept
: loop_(loop), fd_(fd)
{
    assert(loop);
    assert(fd >= 0);
}

EventChannel::~EventChannel()
{
    assert(!isHandling_);
    assert(isNoneEvent());
}

void EventChannel::updateChannel() noexcept
{
    loop_->assertInLoopThread();

    loop_->updateChannel(this);
}

void EventChannel::handlerEvent()
{
    loop_->assertInLoopThread();

    if (isBind_)
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
    loop_->assertInLoopThread();

    isHandling_ = true;
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
    isHandling_ = false;
}

}