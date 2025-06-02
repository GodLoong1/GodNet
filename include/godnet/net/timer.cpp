#include "godnet/net/timer.hpp"

#include <atomic>

namespace godnet
{

TimerId Timer::GenerateTimerId() noexcept
{
    static std::atomic<TimerId> onlyId{0};
    return ++onlyId;
}

Timer::Timer(TimerTimePoint expiration,
             TimerDuration interval,
             TimerCallback&& callback) noexcept
: id_(GenerateTimerId()),
  expiration_(expiration),
  interval_(interval),
  callback_(std::move(callback))
{
}

}