#include "godnet/network/event_loop.hpp"

#ifdef __linux__
    #include <sys/eventfd.h>
    #include <unistd.h>
#else
#endif

#include <cassert>
#include <chrono>

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
    assert(!currentThreadLoop);
    currentThreadLoop = this;

#ifdef __linux__
    wakeupFd_ = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    assert(wakeupFd_ >= 0);
    wakeupChannel_ = std::make_unique<EventChannel>(this, wakeupFd_);
    wakeupChannel_->setReadCallback([this] {
        std::uint64_t val{};
        ::read(wakeupFd_, &val, sizeof(val));
    });
    wakeupChannel_->enableReading();
#else
    poller_->setPostEvent([](std::uint64_t event) {
        assert(event == 1);
    });
#endif

    updateTime();
}

EventLoop::~EventLoop()
{
    assertInLoopThread();

#ifdef __linux__
    ::close(wakeupFd_);
    wakeupChannel_->disableAll();
#else
#endif
    currentThreadLoop = nullptr;
}

void EventLoop::start()
{
    assertInLoopThread();

    assert(status_.load(std::memory_order_acquire) != Status::RUNNING);
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

        updateTime();
        // FD事件
        channels_.clear();
        poller_->pollEvents(channels_, timeout);
        for (auto* channel : channels_)
        {
            channel->handlerEvent();
        }

        updateTime();
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
    if (!isInLoopThread() ||
        status_.load(std::memory_order_acquire) == Status::STOP)
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
#ifdef __linux__
    ssize_t i = ::write(wakeupFd_, &val, sizeof(val));
    (void)i;
#else
    poller_->postEvent(1);
#endif
}

void EventLoop::updateTime() noexcept
{
    using namespace std::chrono;
    steadyTimeMs_ = duration_cast<milliseconds>(
        steady_clock::now().time_since_epoch()).count();
}

}