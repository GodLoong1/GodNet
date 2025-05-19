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
using TimerCallback = std::function<void(TimerId)>;
using namespace std::chrono_literals;

class Timer
{
public:
    Timer(std::chrono::milliseconds expiration,
          std::chrono::milliseconds interval,
          TimerCallback&& callback) noexcept;

    bool isRepeat() const noexcept
    {
        return interval_ > 0ms;
    }
    
    TimerId id() const noexcept
    {
        return id_;
    }

    std::chrono::milliseconds expiration() const noexcept
    {
        return expiration_;
    }

    std::chrono::milliseconds interval() const noexcept
    {
        return interval_;
    }

    void resetExpiration(std::chrono::milliseconds expiration) noexcept
    {
        expiration_ = expiration + interval_;
    }

    void run() const noexcept
    {
        callback_(id_);
    }

private:
    static TimerId GenerateTimerId() noexcept;

    const TimerId id_{};
    std::chrono::milliseconds expiration_{};
    const std::chrono::milliseconds interval_{};
    const TimerCallback callback_{};
};

inline bool operator>(const TimerPtr& lhs, const TimerPtr& rhs) noexcept
{
    return lhs->expiration() > rhs->expiration();
}

}

#endif
