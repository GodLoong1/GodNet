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

TimerId TimerQueue::addTimer(TimerTimePoint expiration,
                             TimerDuration interval,
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

int TimerQueue::getNextTimeout() noexcept
{
    loop_->assertInLoopThread();

    if (timers_.empty())
    {
        return -1;
    }

    auto now = Timer::Now();
    const auto& timerPtr = timers_.top();
    if (timerPtr->expiration() <= now)
    {
        return 0;
    }

    std::uint64_t diff = (timerPtr->expiration() - now).count();
    if (diff > std::numeric_limits<int>::max())
    {
        diff = std::numeric_limits<int>::max();
    }
    return static_cast<int>(diff);
}

void TimerQueue::handlerTimer()
{
    loop_->assertInLoopThread();

    auto now = Timer::Now();
    auto expireds = getExpiredTimers(now);
    for (const auto& timerPtr : expireds)
    {
        if (timerIds_.find(timerPtr->id()) != timerIds_.end())
        {
            timerPtr->run();
        }
    }
    resetExpiredTimers(expireds, now);
}

std::vector<TimerPtr> TimerQueue::getExpiredTimers(TimerTimePoint now) noexcept
{
    loop_->assertInLoopThread();

    std::vector<TimerPtr> expiredTimers;
    while (!timers_.empty())
    {
        const auto& timerPtr = timers_.top();
        if (timerPtr->expiration() <= now)
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
    TimerTimePoint now) noexcept
{
    loop_->assertInLoopThread();

    for (const auto& timerPtr : expiredTimers)
    {
        if (timerIds_.find(timerPtr->id()) != timerIds_.end())
        {
            if (timerPtr->isRepeat())
            {
                timerPtr->resetExpiration(now);
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