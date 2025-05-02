#include "godnet/network/event_loop.hpp"

#include <atomic>
#include <csignal>

#include "godnet/util/debug.hpp"
#include "godnet/util/system.hpp"

#include "godnet/network/event_channel.hpp"
#include "godnet/network/event_poller.hpp"

#if defined(GODNET_LINUX)
    #include <sys/eventfd.h>
    #include <unistd.h>
#elif defined(GODNET_WIN)

#endif

namespace godnet
{

thread_local EventLoop* currentThreadLoop{};

EventLoop::EventLoop()
: threadId_(system::getThreadId()),
  poller_(std::make_unique<EventPoller>(this))
{
    if (currentThreadLoop)
    {
        GODNET_THROW_RUNERR("EventLoop is already created in this thread");
    }
    currentThreadLoop = this;

#if defined(GODNET_LINUX)
    if ((wakeupFds_[0] = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)) < 0)
    {
        GODNET_THROW_RUNERR("eventfd() failed");
    }
    wakeupChannel_ = std::make_unique<EventChannel>(this, wakeupFds_[0]);
    wakeupChannel_->setReadCallback([this] {
        std::uint64_t val{};
        ::read(wakeupFds_[0], &val, sizeof(val));
    });
    wakeupChannel_->enableReading();
#elif defined(GODNET_WIN)
    // TODO
#endif
}

EventLoop::~EventLoop()
{
    assertInLoopThread();

#if defined(GODNET_LINUX)
    ::close(wakeupFds_[0]);
    wakeupChannel_->disableAll();
#elif defined(GODNET_WIN)
    // TODO
#endif
    currentThreadLoop = nullptr;
}

void EventLoop::start()
{
    assertInLoopThread();

    if (status_.load(std::memory_order_acquire) == Status::RUNNING)
    {
        GODNET_THROW_RUNERR("EventLoop is already started");
    }
    status_.store(Status::RUNNING, std::memory_order_release);

    while (status_.load(std::memory_order_acquire) == Status::RUNNING)
    {
        // FD事件
        channels_.clear();
        poller_->pollEvents(channels_, -1);
        for (auto* channel : channels_)
        {
            channel->handlerEvent();
        }

        // 处理自定义事件
        while (!customEvents_.empty())
        {
            EventCallback cb;
            while (customEvents_.dequeue(cb))
            {
                cb();
            }
        }
    }
}

void EventLoop::stop()
{
    status_.store(Status::STOP, std::memory_order_release);
    if (!isInLoopThread())
    {
        wakeup();
    }
}

void EventLoop::runInLoop(EventCallback&& func)
{
    if (isInLoopThread())
    {
        func();
    }
    else
    {
        queueInLoop(std::move(func));
    }
}

void EventLoop::queueInLoop(EventCallback&& func)
{
    customEvents_.enqueue(std::move(func));
    if (!isInLoopThread() || status_.load(std::memory_order_acquire) == Status::STOP)
    {
        wakeup();
    }
}

bool EventLoop::isInLoopThread() const noexcept
{
    return threadId_ == system::getThreadId();
}

void EventLoop::assertInLoopThread()
{
    if (!isInLoopThread())
    {
        std::terminate();
    }
}

void EventLoop::updateChannel(EventChannel* channel)
{
    assertInLoopThread();

    poller_->updateChannel(channel);
}

void EventLoop::wakeup()
{
    std::uint64_t val{1};
#if defined(GODNET_LINUX)
    ::write(wakeupFds_[0], &val, sizeof(val));
#elif defined(GODNET_WIN)
    // TODO
#endif
}
}