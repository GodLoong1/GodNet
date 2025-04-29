#ifndef GODNET_NETWORK_EVENT_BASE_HPP
#define GODNET_NETWORK_EVENT_BASE_HPP

#include "godnet/config.hpp"

#include <cstdint>
#include <functional>
#include <chrono>
#include <memory>

namespace godnet
{

using EventCallback = std::function<void()>;
using SteadyTimerPoint = std::chrono::steady_clock::time_point;
using SystemTimerPoint = std::chrono::system_clock::time_point;
using TimerInterval = std::chrono::microseconds;

class EventLoop;

enum class GODNET_EXPORT EventType : std::uint8_t
{
    CUSTOM = 1,
    IDLE,
    CHANNEL,
    STEADY_TIMER,
    SYSTEM_TIMER,
    SIGNAL,
};

class GODNET_EXPORT EventId
{
public:
    EventId(EventType type, std::uint64_t count) noexcept
    {
        id_ = (static_cast<std::uint64_t>(type) << 56) | (count & 0x00FFFFFFFFFFFFFF);
    }

    EventType eventType() const noexcept
    {
        return static_cast<EventType>((id_ >> 56) & 0xFF);
    }

    std::uint64_t count() const noexcept
    {
        return id_ & 0x00FFFFFFFFFFFFFF;
    }

    std::uint64_t id() const noexcept
    {
        return id_;
    }

    operator std::uint64_t() const noexcept
    {
        return id_;
    }

    static EventId GenerateEventId(EventType type);

private:
    std::uint64_t id_;
};

class GODNET_EXPORT EventBase
{
public:
    explicit EventBase(EventType type) noexcept
    : type_(type) {}

    EventType getEventType() const noexcept
    {
        return type_;
    }

private:
    EventType type_;
};

class GODNET_EXPORT CustomEvent : public EventBase
{
public:
    explicit CustomEvent(EventCallback&& callback) noexcept
    : EventBase(EventType::CUSTOM), callback_(std::move(callback)) {}

private:
    EventCallback callback_;
};

class GODNET_EXPORT IdleEvent : public EventBase
{
public:
    explicit IdleEvent(EventCallback&& callback) noexcept
    : EventBase(EventType::IDLE), callback_(std::move(callback)) {}

private:
    EventCallback callback_;
};

class GODNET_EXPORT SignalEvent : public EventBase
{
    explicit SignalEvent(EventCallback&& callback) noexcept
    : EventBase(EventType::SIGNAL), callback_(std::move(callback)) {}

private:
    EventCallback callback_;
};

class GODNET_EXPORT SteadyTimerEvent : public EventBase
{
public:
    SteadyTimerEvent(EventCallback&& callback,
                     const SteadyTimerPoint& expiration,
                     const TimerInterval& interval) noexcept
    : EventBase(EventType::STEADY_TIMER),
      callback_(std::move(callback)),
      expiration_(expiration),
      interval_(interval) {}

private:
    EventCallback callback_;
    SteadyTimerPoint expiration_;
    TimerInterval interval_;
};

class GODNET_EXPORT SystemTimerEvent : public EventBase
{
public:
    SystemTimerEvent(EventCallback&& callback,
                     const SystemTimerPoint& expiration,
                     const TimerInterval& interval) noexcept
    : EventBase(EventType::SYSTEM_TIMER),
      callback_(std::move(callback)),
      expiration_(expiration),
      interval_(interval) {}

private:
    EventCallback callback_;
    SystemTimerPoint expiration_;
    TimerInterval interval_;
};

class GODNET_EXPORT ChannelEvent : EventBase
{
public:
    static const std::uint32_t NONE_EVENT;
    static const std::uint32_t READ_EVENT;
    static const std::uint32_t WRITE_EVENT;

    ChannelEvent(EventLoop* loop, int fd)
    : EventBase(EventType::CHANNEL), loop_(loop), fd_(fd) { }

    ~ChannelEvent();

    void setReadCallback(EventCallback&& callback) noexcept
    {
        read_callback_ = std::move(callback);
    }

    void setReadCallback(const EventCallback& callback)
    {
        read_callback_ = callback;
    }

    void setWriteCallback(EventCallback&& callback) noexcept
    {
        write_callback_ = std::move(callback);
    }

    void setWriteCallback(const EventCallback& callback)
    {
        write_callback_ = callback;
    }

    void setErrorCallback(EventCallback&& callback) noexcept
    {
        error_callback_ = std::move(callback);
    }

    void setErrorCallback(const EventCallback& callback)
    {
        error_callback_ = callback;
    }

    void setCloseCallback(EventCallback&& callback) noexcept
    {
        close_callback_ = std::move(callback);
    }

    void setCloseCallback(const EventCallback& callback)
    {
        close_callback_ = callback;
    }

    void setEventCallback(EventCallback&& callback) noexcept
    {
        event_callback_ = std::move(callback);
    }

    void setEventCallback(const EventCallback& callback)
    {
        event_callback_ = callback;
    }

    void bindWeakPtr(std::weak_ptr<void>&& object) noexcept
    {
        is_bind = true;
        object_handler_ = std::move(object);
    }

    void bindWeakPtr(const std::weak_ptr<void>& object)
    {
        is_bind = true;
        object_handler_ = object;
    }

    EventLoop* getEventLoop() const noexcept
    {
        return loop_;
    }

    int getFd() const noexcept
    {
        return fd_;
    }

    std::uint32_t getEvents() const noexcept
    {
        return events_;
    }

    std::uint32_t getRevents() const noexcept
    {
        return revents_;
    }

    void setRevents(std::uint32_t revents) noexcept
    {
        revents_ = revents;
    }

    void enableReading()
    {
        events_ |= READ_EVENT;
        update();
    }

    void enableWriting()
    {
        events_ |= WRITE_EVENT;
        update();
    }

    void disableReading()
    {
        events_ &= ~READ_EVENT;
        update();
    }

    void disableWriting()
    {
        events_ &= ~WRITE_EVENT;
        update();
    }

    void disableAll()
    {
        events_ = NONE_EVENT;
        update();
    }

    bool isReading() const noexcept
    {
        return events_ & READ_EVENT;
    }

    bool isWriting() const noexcept
    {
        return events_ & WRITE_EVENT;
    }

    bool isNoneEvent() const noexcept
    {
        return events_ == NONE_EVENT;
    }

    void update();
    void handlerEvent();

private:
    void handlerEventSafe();

    EventLoop* loop_{};
    int fd_{};
    std::uint32_t events_{};
    std::uint32_t revents_{};

    EventCallback read_callback_{};
    EventCallback write_callback_{};
    EventCallback error_callback_{};
    EventCallback close_callback_{};
    EventCallback event_callback_{};

    bool is_bind{};
    bool is_handling_{};
    std::weak_ptr<void> object_handler_{};
};


}

#endif
