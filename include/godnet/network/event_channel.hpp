#ifndef GODNET_NETWORK_EVENT_CHANNEL_HPP
#define GODNET_NETWORK_EVENT_CHANNEL_HPP

#include "godnet/config.hpp"
#include "godnet/util/noncopyable.hpp"

#include <cstdint>
#include <functional>
#include <memory>

namespace godnet
{

class EventLoop;

class GODNET_EXPORT EventChannel : Noncopyable
{
public:
    static const std::uint32_t NONE_EVENT;
    static const std::uint32_t READ_EVENT;
    static const std::uint32_t WRITE_EVENT;

    using EventCallback = std::function<void()>;

    EventChannel(EventLoop* loop, int fd);
    ~EventChannel();

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

    void disableReading()
    {
        events_ &= ~READ_EVENT;
        update();
    }

    bool isReading() const noexcept
    {
        return events_ & READ_EVENT;
    }

    void enableWriting()
    {
        events_ |= WRITE_EVENT;
        update();
    }

    void disableWriting()
    {
        events_ &= ~WRITE_EVENT;
        update();
    }

    bool isWriting() const noexcept
    {
        return events_ & WRITE_EVENT;
    }

    bool isNone() const noexcept
    {
        return events_ == NONE_EVENT;
    }

    void disableAll()
    {
        events_ = NONE_EVENT;
        update();
    }

    void update();
    void handlerEvent();

private:
    void handlerEventSafe();

    EventLoop* const loop_{};
    const int fd_{};
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
