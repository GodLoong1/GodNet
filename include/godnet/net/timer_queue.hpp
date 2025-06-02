#ifndef GODNET_NET_TIMER_QUEUE_HPP
#define GODNET_NET_TIMER_QUEUE_HPP

#include <queue>
#include <unordered_set>

#include "godnet/net/timer.hpp"

namespace godnet
{

class EventLoop;

class TimerQueue
{
public:
    explicit TimerQueue(EventLoop* loop);

    TimerId addTimer(TimerTimePoint expiration,
                     TimerDuration interval,
                     TimerCallback&& callback) noexcept;
    void delTimer(TimerId id) noexcept;

    int getNextTimeout() noexcept;
    void handlerTimer();

private:
    std::vector<TimerPtr> getExpiredTimers(TimerTimePoint now) noexcept;
    void resetExpiredTimers(const std::vector<TimerPtr>& expiredTimers,
                            TimerTimePoint now) noexcept;
    void addTimerInLoop(const TimerPtr& timerPtr) noexcept;
    void delTimerInLoop(TimerId id) noexcept;

    EventLoop* loop_{};
    std::priority_queue<TimerPtr,
                        std::vector<TimerPtr>,
                        std::greater<TimerPtr>> timers_{};
    std::unordered_set<TimerId> timerIds_{};
};

}

#endif
