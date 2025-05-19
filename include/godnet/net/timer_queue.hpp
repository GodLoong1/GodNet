#ifndef GODNET_NET_TIMER_QUEUE_HPP
#define GODNET_NET_TIMER_QUEUE_HPP

#include <chrono>
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

    TimerId addTimer(std::chrono::steady_clock::time_point expiration,
                     std::chrono::milliseconds interval,
                     TimerCallback&& callback) noexcept;
    void delTimer(TimerId id) noexcept;

    int getNextTimeout() noexcept;
    void handlerTimer();

private:
    std::vector<TimerPtr> getExpiredTimers(std::chrono::milliseconds timeout) noexcept;
    void resetExpiredTimers(const std::vector<TimerPtr>& expiredTimers,
                            std::chrono::milliseconds timeout) noexcept;
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
