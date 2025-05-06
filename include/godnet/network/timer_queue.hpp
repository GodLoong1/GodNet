#ifndef GODNET_NETWORK_TIMER_QUEUE_HPP
#define GODNET_NETWORK_TIMER_QUEUE_HPP

#include "godnet/config.hpp"

#include <queue>
#include <unordered_set>

#include "godnet/network/timer.hpp"

namespace godnet
{

class EventLoop;

class GODNET_EXPORT TimerQueue
{
public:
    explicit TimerQueue(EventLoop* loop);

    TimerId addTimer(std::uint64_t expiration,
                     std::uint64_t interval,
                     TimerCallback&& callback) noexcept;
    void delTimer(TimerId id) noexcept;

    int getNextTimeout(std::uint64_t timeout);
    void handlerTimer(std::uint64_t timeout);

private:
    std::vector<TimerPtr> getExpiredTimers(std::uint64_t timeout) noexcept;
    void resetExpiredTimers(const std::vector<TimerPtr>& expiredTimers,
                            std::uint64_t timeout) noexcept;
    void addTimerInLoop(const TimerPtr& timerPtr) noexcept;
    void delTimerInLoop(TimerId id) noexcept;

    EventLoop* loop_{};
    std::priority_queue<TimerPtr,
                        std::vector<TimerPtr>,
                        std::greater<TimerPtr>> timers_;
    std::unordered_set<TimerId> timerIds_{};
};

}

#endif
