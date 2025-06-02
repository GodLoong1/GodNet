#ifndef GODNET_NET_TIMER_HPP
#define GODNET_NET_TIMER_HPP

#include <cstdint>
#include <functional>
#include <memory>
#include <chrono>

namespace godnet
{

using TimerId = std::uint64_t;
class Timer;
using TimerPtr = std::shared_ptr<Timer>;
using TimerCallback = std::function<void()>;
using TimerDuration = std::chrono::milliseconds;
using TimerTimePoint =
    std::chrono::time_point<std::chrono::steady_clock, TimerDuration>;
using namespace std::chrono_literals;

class Timer
{
public:
    static TimerTimePoint Now() noexcept
    {
        return std::chrono::time_point_cast<TimerDuration>(
            std::chrono::steady_clock::now());
    }

    Timer(TimerTimePoint expiration,
          TimerDuration interval,
          TimerCallback&& callback) noexcept;

    bool isRepeat() const noexcept
    {
        return interval_ > TimerDuration::zero();
    }
    
    TimerId id() const noexcept
    {
        return id_;
    }

    TimerTimePoint expiration() const noexcept
    {
        return expiration_;
    }

    TimerDuration interval() const noexcept
    {
        return interval_;
    }

    void resetExpiration(TimerTimePoint expiration) noexcept
    {
        expiration_ = expiration + interval_;
    }

    void run() const noexcept
    {
        callback_();
    }

private:
    static TimerId GenerateTimerId() noexcept;

    const TimerId id_;
    TimerTimePoint expiration_;
    const TimerDuration interval_;
    const TimerCallback callback_;
};

inline bool operator>(const TimerPtr& lhs, const TimerPtr& rhs) noexcept
{
    if (lhs->expiration() != rhs->expiration())
    {
        return lhs->expiration() > rhs->expiration();
    }
    return lhs->id() > rhs->id();
}

}

#endif