#include "godnet/net/event_loop.hpp"

#ifdef __linux__
    #include <sys/eventfd.h>
    #include <unistd.h>
#endif

#include <cassert>
#include <exception>

#include "godnet/util/system.hpp"
#include "godnet/util/logger.hpp"
#include "godnet/net/event_channel.hpp"
#include "godnet/net/event_poller.hpp"
#include "godnet/net/timer_queue.hpp"

namespace godnet
{

thread_local EventLoop* currentThreadLoop{};

EventLoop* EventLoop::GetLoop() noexcept
{
    return currentThreadLoop;
}

EventLoop::EventLoop() noexcept
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
    GODNET_LOG_TRACE("new EventLoop threadId: {}", threadId_);
}

EventLoop::~EventLoop()
{
    assertInLoopThread();

#ifdef __linux__
    wakeupChannel_->disableAll();
    ::close(wakeupFd_);
#endif
    currentThreadLoop = nullptr;
}

void EventLoop::start()
{
    assertInLoopThread();

    assert(!isRunning());
    status_.store(Status::running, std::memory_order_release);

    while (isRunning())
    {
        timers_->handlerTimer();

        channels_.clear();
        poller_->pollEvents(channels_, timers_->getNextTimeout());
        for (auto* channel : channels_)
        {
            channel->handlerEvent();
        }

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

void EventLoop::stop() noexcept
{
    assert(!isStop());
    status_.store(Status::stop, std::memory_order_release);
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

void EventLoop::queueInLoop(EventCallback&& func) noexcept
{
    customEvents_.enqueue(std::move(func));
    if (!isInLoopThread() || isStop())
    {
        wakeup();
    }
}

TimerId EventLoop::runTimer(TimerDuration delay,
                            TimerDuration interval,
                            TimerCallback&& callback) noexcept
{
    return timers_->addTimer(Timer::Now() + delay,
                             interval,
                             std::move(callback));
}

TimerId EventLoop::runTimerAfter(TimerDuration delay,
                                 TimerCallback&& callback) noexcept
{
    return runTimer(delay, TimerDuration::zero(), std::move(callback));
}

TimerId EventLoop::runTimerEvery(TimerDuration interval,
                                 TimerCallback&& callback) noexcept
{
    return runTimer(interval, interval, std::move(callback));
}

void EventLoop::cancelTimer(TimerId id) noexcept
{
    timers_->delTimer(id);
}

bool EventLoop::isInLoopThread() const noexcept
{
    return threadId_ == system::getThreadId();
}

void EventLoop::assertInLoopThread() const noexcept
{
    if (!isInLoopThread())
    {
        std::terminate();
    }
}

void EventLoop::updateChannel(EventChannel* channel) noexcept
{
    assert(this == channel->loop());
    poller_->updateChannel(channel);
}

void EventLoop::wakeup()
{
    std::uint64_t val{1};
#ifdef __linux__
    [[maybe_unused]] ssize_t ret = ::write(wakeupFd_, &val, sizeof(val));
    assert(ret == sizeof(val));
#else
    poller_->postEvent(val);
#endif
}

}