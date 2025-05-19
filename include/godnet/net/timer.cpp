#include "godnet/net/timer.hpp"

#include <atomic>

namespace godnet
{

TimerId Timer::GenerateTimerId() noexcept
{
    static std::atomic<TimerId> id{0};
    return ++id;
}

Timer::Timer(std::chrono::milliseconds expiration,
             std::chrono::milliseconds interval,
             TimerCallback&& callback) noexcept
: id_(GenerateTimerId()),
  expiration_(expiration),
  interval_(interval),
  callback_(std::move(callback))
{
}

}