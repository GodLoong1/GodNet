#ifndef GODNET_NETWORK_TIMER_QUEUE_HPP
#define GODNET_NETWORK_TIMER_QUEUE_HPP

#include "godnet/config.hpp"

#include <queue>
#include <unordered_set>

namespace godnet
{

class SteadyTimerEvent;
class SystemTimerEvent;

class GODNET_EXPORT TimerQueue
{
public:

private:
    std::priority_queue<SteadyTimerEvent> timers_;
    std::unordered_set<int> cancelEventIds_;
};

}

#endif
