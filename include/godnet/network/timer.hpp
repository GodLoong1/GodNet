#ifndef GODNET_NETWORK_TIMER_HPP
#define GODNET_NETWORK_TIMER_HPP

#include "godnet/config.hpp"

#include <cstdint>
#include <functional>
#include <memory>

namespace godnet
{

using TimerId = std::uint64_t;
class Timer;
using TimerPtr = std::shared_ptr<Timer>;
using TimerCallback = std::function<void()>;

class GODNET_EXPORT Timer
{
public:
    Timer(std::uint64_t expiration,
          std::uint64_t interval,
          TimerCallback&& callback) noexcept;

    bool isRepeat() const noexcept
    {
        return interval_ > 0;
    }
    
    TimerId id() const noexcept
    {
        return id_;
    }

    std::uint64_t expiration() const noexcept
    {
        return expiration_;
    }

    std::uint64_t interval() const noexcept
    {
        return interval_;
    }

    void callback() const noexcept
    {
        callback_();
    }

    void resetExpiration(std::uint64_t expiration) noexcept
    {
        expiration_ = expiration + interval_;
    }

private:
    static TimerId GenerateTimerId() noexcept;

    const TimerId id_{};
    std::uint64_t expiration_{};
    const std::uint64_t interval_{};
    const TimerCallback callback_{};
};

inline bool operator>(const TimerPtr& lhs, const TimerPtr& rhs) noexcept
{
    return lhs->expiration() > rhs->expiration();
}

}

#endif
