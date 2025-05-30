#include "godnet/net/timer_queue.hpp"

#include <cassert>
#include <climits>

#include "godnet/net/event_loop.hpp"

namespace godnet
{

TimerQueue::TimerQueue(EventLoop* loop)
: loop_(loop)
{
    assert(loop);
}

TimerId TimerQueue::addTimer(std::chrono::steady_clock::time_point expiration,
                             std::chrono::milliseconds interval,
                             TimerCallback&& callback) noexcept
{
    auto timerPtr = std::make_shared<Timer>(std::chrono::duration_cast<
                                            std::chrono::milliseconds>(
                                            expiration.time_since_epoch()),
                                            interval,
                                            std::move(callback));
    if (loop_->isInLoopThread())
    {
        addTimerInLoop(timerPtr);
    }
    else
    {
        loop_->queueInLoop([this, timerPtr] {
            addTimerInLoop(timerPtr);
        });
    }
    return timerPtr->id();
}

void TimerQueue::delTimer(TimerId id) noexcept
{
    if (loop_->isInLoopThread())
    {
        delTimerInLoop(id);
    }
    else
    {
        loop_->queueInLoop([this, id] {
            delTimerInLoop(id);
        });
    }
}

int TimerQueue::getNextTimeout() noexcept
{
    loop_->assertInLoopThread();

    if (timers_.empty())
    {
        return -1;
    }

    auto timeout =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch());

    const auto& timerPtr = timers_.top();
    if (timerPtr->expiration() <= timeout)
    {
        return 0;
    }

    std::uint64_t diff = (timerPtr->expiration() - timeout).count();
    if (diff > std::numeric_limits<int>::max())
    {
        diff = std::numeric_limits<int>::max();
    }
    return static_cast<int>(diff);
}

void TimerQueue::handlerTimer()
{
    loop_->assertInLoopThread();

    auto timeout =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch());

    auto expireds = getExpiredTimers(timeout);
    for (const auto& timerPtr : expireds)
    {
        if (timerIds_.find(timerPtr->id()) != timerIds_.end())
        {
            timerPtr->run();
        }
    }
    resetExpiredTimers(expireds, timeout);
}

std::vector<TimerPtr> TimerQueue::getExpiredTimers(
    std::chrono::milliseconds timeout) noexcept
{
    loop_->assertInLoopThread();

    std::vector<TimerPtr> expiredTimers;
    while (!timers_.empty())
    {
        const auto& timerPtr = timers_.top();
        if (timerPtr->expiration() <= timeout)
        {
            expiredTimers.emplace_back(timerPtr);
            timers_.pop();
        }
        else
        {
            break;
        }
    }
    return expiredTimers;
}

void TimerQueue::resetExpiredTimers(
    const std::vector<TimerPtr>& expiredTimers,
    std::chrono::milliseconds timeout) noexcept
{
    loop_->assertInLoopThread();

    for (const auto& timerPtr : expiredTimers)
    {
        if (timerIds_.find(timerPtr->id()) != timerIds_.end())
        {
            if (timerPtr->isRepeat())
            {
                timerPtr->resetExpiration(timeout);
                timers_.emplace(timerPtr);
            }
            else
            {
                timerIds_.erase(timerPtr->id());
            }
        }
    }
}

void TimerQueue::addTimerInLoop(const TimerPtr& timerPtr) noexcept
{
    loop_->assertInLoopThread();

    timerIds_.insert(timerPtr->id());
    timers_.emplace(timerPtr);
}

void TimerQueue::delTimerInLoop(TimerId id) noexcept
{
    loop_->assertInLoopThread();

    [[maybe_unused]] auto ret = timerIds_.erase(id);
    assert(ret);
}

}