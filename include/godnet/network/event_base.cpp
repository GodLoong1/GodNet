#include "godnet/network/event_base.hpp"

#if defined(GODNET_WIN)
    #include "wepoll.h"
#elif defined(GODNET_LINUX)
    #include <sys/epoll.h>
#endif

#include <atomic>

#include "godnet/util/debug.hpp"
#include "godnet/network/event_loop.hpp"

namespace godnet
{

EventId EventId::GenerateEventId(EventType type)
{
    switch (type)
    {
    case EventType::CUSTOM:
        static std::atomic<std::uint64_t> customId{0};
        return EventId(type, ++customId);
    case EventType::IDLE:
        static std::atomic<std::uint64_t> idleId{0};
        return EventId(type, ++idleId);
    case EventType::CHANNEL:
        static std::atomic<std::uint64_t> channelId{0};
        return EventId(type, ++channelId);
    case EventType::STEADY_TIMER:
        static std::atomic<std::uint64_t> steadyTimerId{0};
        return EventId(type, ++steadyTimerId);
    case EventType::SYSTEM_TIMER:
        static std::atomic<std::uint64_t> systemTimerId{0};
        return EventId(type, ++systemTimerId);
    case EventType::SIGNAL:
        static std::atomic<std::uint64_t> signalId{0};
        return EventId(type, ++signalId);
    default:
        GODNET_THROW_RUNERR("Invalid EventType");
    }
}

const std::uint32_t ChannelEvent::NONE_EVENT = 0;
const std::uint32_t ChannelEvent::READ_EVENT = EPOLLIN | EPOLLPRI;
const std::uint32_t ChannelEvent::WRITE_EVENT = EPOLLOUT;

ChannelEvent::~ChannelEvent()
{
    GODNET_ASSERT(!is_handling_ && "EventChannel is handling events");
}

void ChannelEvent::update()
{
    loop_->assertInLoop();
    loop_->updateChannel(this);
}

void ChannelEvent::handlerEvent()
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

void ChannelEvent::handlerEventSafe()
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
