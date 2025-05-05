#include "godnet/network/timer_queue.hpp"

#include <limits>

#include "godnet/network/event_loop.hpp"

namespace godnet
{

TimerQueue::TimerQueue(EventLoop* loop)
: loop_(loop)
{
}

TimerId TimerQueue::addTimer(std::uint64_t expiration,
                             std::uint64_t interval,
                             TimerCallback&& callback) noexcept
{
    auto timerPtr = std::make_shared<Timer>(expiration,
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

int TimerQueue::getNextTimeout(std::uint64_t timeout)
{
    if (!timers_.empty())
    {
        const auto& timerPtr = timers_.top();
        if (timerPtr->expiration() <= timeout)
        {
            return 0;
        }
        std::uint64_t diff = timerPtr->expiration() - timeout;
        if (diff > std::numeric_limits<int>::max())
        {
            diff = std::numeric_limits<int>::max();
        }
        return static_cast<int>(diff);
    }
    return -1;
}

void TimerQueue::handlerTimer(std::uint64_t timeout)
{
    loop_->assertInLoopThread();

    auto expireds = getExpiredTimers(timeout);
    for (const auto& timerPtr : expireds)
    {
        if (timerIds_.find(timerPtr->id()) != timerIds_.end())
        {
            timerPtr->callback();
        }
    }
    resetExpiredTimers(expireds, timeout);
}

std::vector<TimerPtr> TimerQueue::getExpiredTimers(
    std::uint64_t timeout) noexcept
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
    std::uint64_t timeout) noexcept
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

    timerIds_.erase(id);
}

}