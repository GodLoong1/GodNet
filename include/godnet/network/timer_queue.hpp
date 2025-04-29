#ifndef GODNET_NETWORK_TIMER_QUEUE_HPP
#define GODNET_NETWORK_TIMER_QUEUE_HPP

#include "godnet/config.hpp"

#include <queue>
#include <unordered_set>

namespace godnet
{

class GODNET_EXPORT TimerQueue
{
public:

private:
    std::priority_queue<TimerPtr, TimerPtrVec, std::greater<TimerPtr>> timers_;
    std::unordered_set<int> cancelIds_;
};

}

#endif
