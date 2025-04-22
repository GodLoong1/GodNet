#include "godnet/network/event_loop.hpp"

#include "godnet/util/debug.hpp"
#include "godnet/util/system.hpp"

#include "godnet/network/event_channel.hpp"
#include "godnet/network/event_poller.hpp"
#include <atomic>

#if defined(GODNET_LINUX)
    #include <sys/eventfd.h>
    #include <unistd.h>
#elif defined(GODNET_WIN)

#endif

namespace godnet
{

thread_local EventLoop* current_thread_loop{};

EventLoop::EventLoop()
: thread_id_(system::getThreadId()),
  poller_(std::make_unique<EventPoller>(this))
{
    if (current_thread_loop)
    {
        GODNET_THROW_RUNERR("EventLoop already exists in this thread");
    }
    current_thread_loop = this;

    if ((wakeup_fd_ = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)) < 0)
    {
        GODNET_THROW_RUNERR("eventfd() failed");
    }
    wakeup_channel_ = std::make_unique<EventChannel>(this, wakeup_fd_);
    wakeup_channel_->setReadCallback([this] {
        uint64_t val{};
        if (::read(wakeup_fd_, &val, sizeof(val)) < 0)
        {
            GODNET_THROW_RUNERR("read() failed");
        }
    });
    wakeup_channel_->enableReading();
}

EventLoop::~EventLoop()
{
#if defined(GODNET_LINUX)
    close(wakeup_fd_);
#endif
    current_thread_loop = nullptr;
}

void EventLoop::loop()
{
    assertInLoop();
    if (looping_)
    {
        GODNET_THROW_RUNERR("EventLoop is already looping");
    }
    looping_.store(true, std::memory_order_release);
    quit_.store(false, std::memory_order_release);

    while (!quit_.load(std::memory_order_acquire))
    {
        ready_channel_vec_.clear();
        poller_->poll(ready_channel_vec_);

        for (auto* channel : ready_channel_vec_)
        {
            channel->handlerEvent();
        }

        // 在循环中插入队列
        while (!event_callback_queue_.empty())
        {
            EventCallback cb;
            while (event_callback_queue_.dequeue(cb))
            {
                cb();
            }
        }
    }
}

void EventLoop::quit()
{
    quit_.store(true, std::memory_order_release);
    if (!isInLoop())
    {
        wakeup();
    }
}

bool EventLoop::isInLoop() const noexcept
{
    return thread_id_ == system::getThreadId();
}

void EventLoop::assertInLoop()
{
    if (!isInLoop())
    {
        std::terminate();
    }
}

void EventLoop::updateChannel(EventChannel* channel)
{
    poller_->update(channel);
}

void EventLoop::wakeup()
{
    std::uint64_t val{1};
    ::write(wakeup_fd_, &val, sizeof(val));
}

void EventLoop::queueInLoop(const EventCallback& func)
{
    event_callback_queue_.enqueue(func);
    if (!isInLoop() || !looping_.load(std::memory_order_acquire))
    {
        wakeup();
    }
}

void EventLoop::queueInLoop(EventCallback&& func)
{
    event_callback_queue_.enqueue(std::move(func));
    if (!isInLoop() || !looping_.load(std::memory_order_acquire))
    {
        wakeup();
    }
}

}