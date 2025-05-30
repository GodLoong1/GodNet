#ifndef GODNET_NET_EVENT_CHANNEL_HPP
#define GODNET_NET_EVENT_CHANNEL_HPP

#include <cstdint>
#include <memory>
#include <functional>

#include "godnet/util/noncopyable.hpp"

namespace godnet
{

class EventLoop;

class EventChannel : Noncopyable
{
public:
    using EventCallback = std::function<void()>;

    static const std::uint32_t NONE_EVENT;
    static const std::uint32_t READ_EVENT;
    static const std::uint32_t WRITE_EVENT;

    EventChannel(EventLoop* loop, int fd) noexcept;
    ~EventChannel();

    void setReadCallback(EventCallback&& callback) noexcept
    {
        readCallback_ = std::move(callback);
    }

    void setWriteCallback(EventCallback&& callback) noexcept
    {
        writeCallback_ = std::move(callback);
    }

    void setErrorCallback(EventCallback&& callback) noexcept
    {
        errorCallback_ = std::move(callback);
    }

    void setCloseCallback(EventCallback&& callback) noexcept
    {
        closeCallback_ = std::move(callback);
    }

    void setEventCallback(EventCallback&& callback) noexcept
    {
        eventCallback_ = std::move(callback);
    }

    void bindWeakPtr(std::weak_ptr<void>&& object) noexcept
    {
        isBind_ = true;
        bindObject_ = std::move(object);
    }

    EventLoop* loop() const noexcept
    {
        return loop_;
    }

    int fd() const noexcept
    {
        return fd_;
    }

    std::uint32_t events() const noexcept
    {
        return events_;
    }

    std::uint32_t revents() const noexcept
    {
        return revents_;
    }

    void setRevents(std::uint32_t revents) noexcept
    {
        revents_ = revents;
    }

    void enableReading() noexcept
    {
        events_ |= READ_EVENT;
        updateChannel();
    }

    void enableWriting() noexcept
    {
        events_ |= WRITE_EVENT;
        updateChannel();
    }

    void disableReading() noexcept
    {
        events_ &= ~READ_EVENT;
        updateChannel();
    }

    void disableWriting() noexcept
    {
        events_ &= ~WRITE_EVENT;
        updateChannel();
    }

    void disableAll() noexcept
    {
        events_ = NONE_EVENT;
        updateChannel();
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

    void updateChannel() noexcept;
    void handlerEvent();

private:
    void handlerEventSafe();

    EventLoop* const loop_;
    const int fd_;
    std::uint32_t events_{};
    std::uint32_t revents_{};

    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
    EventCallback closeCallback_;
    EventCallback eventCallback_;

    bool isBind_{};
    bool isHandling_{};
    std::weak_ptr<void> bindObject_;
};

}

#endif