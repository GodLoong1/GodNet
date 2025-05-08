#include "godnet/network/event_loop.hpp"

#if defined(GODNET_LINUX)
    #include <sys/eventfd.h>
    #include <unistd.h>
#elif defined(GODNET_WIN)

#endif

#include <csignal>
#include <chrono>

#include "godnet/util/debug.hpp"
#include "godnet/util/system.hpp"
#include "godnet/network/event_channel.hpp"
#include "godnet/network/event_poller.hpp"
#include "godnet/network/timer_queue.hpp"

namespace godnet
{

thread_local EventLoop* currentThreadLoop{};

EventLoop* EventLoop::GetLoop() noexcept
{
    return currentThreadLoop;
}

EventLoop::EventLoop()
: threadId_(system::getThreadId()),
  timers_(std::make_unique<TimerQueue>(this)),
  poller_(std::make_unique<EventPoller>(this))
{
    if (currentThreadLoop)
    {
        GODNET_THROW_RUNERR("EventLoop is already created in this thread");
    }
    currentThreadLoop = this;

#ifdef __linux__
    if ((wakeupFd_ = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)) < 0)
    {
        GODNET_THROW_RUNERR("eventfd() failed");
    }
    wakeupChannel_ = std::make_unique<EventChannel>(this, wakeupFd_);
    wakeupChannel_->setReadCallback([this] {
        std::uint64_t val{};
        ::read(wakeupFd_, &val, sizeof(val));
    });
    wakeupChannel_->enableReading();
#endif

    updateTime();
}

EventLoop::~EventLoop()
{
    assertInLoopThread();

#if defined(GODNET_LINUX)
    ::close(wakeupFd_);
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
        updateTime();

        // 定时器事件
        int timeout = timers_->getNextTimeout(steadyTimeMs_); 
        if (timeout == 0)
        {
            timers_->handlerTimer(steadyTimeMs_);
        }

        // FD事件
        channels_.clear();
        poller_->pollEvents(channels_, timeout);
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

TimerId EventLoop::runTimer(std::uint64_t delay,
                            std::uint64_t interval,
                            TimerCallback&& callback)
{
    return timers_->addTimer(steadyTimeMs_ + delay,
                             interval,
                             std::move(callback));
}

TimerId EventLoop::runTimerAfter(std::uint64_t delay,
                                 TimerCallback&& callback)
{
    return runTimer(delay, 0, std::move(callback));
}

TimerId EventLoop::runTimerEvery(std::uint64_t interval,
                                 TimerCallback&& callback)
{
    return runTimer(interval, interval, std::move(callback));
}

void EventLoop::cancelTimer(TimerId id)
{
    timers_->delTimer(id);
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

void EventLoop::updateTime() noexcept
{
    using namespace std::chrono;
    steadyTimeMs_ = duration_cast<milliseconds>(
        steady_clock::now().time_since_epoch()).count();
}

}