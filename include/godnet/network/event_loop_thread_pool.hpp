#ifndef GODNET_NETWORK_EVENT_LOOP_THREAD_POOL_HPP
#define GODNET_NETWORK_EVENT_LOOP_THREAD_POOL_HPP

#include <vector>
#include <memory>
#include <cstdint>

#include "godnet/util/noncopyable.hpp"

namespace godnet
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : public Noncopyable
{
public:
    EventLoopThreadPool(std::uint32_t threadNum,
                        std::string threadName) noexcept;

    void start() noexcept;
    void stop() noexcept;
    std::vector<EventLoop*> getLoops() noexcept;

private:
    std::vector<std::unique_ptr<EventLoopThread>> loops_;
};

}

#endif
