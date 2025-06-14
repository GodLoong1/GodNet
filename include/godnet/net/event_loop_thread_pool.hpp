#ifndef GODNET_NET_EVENT_LOOP_THREAD_POOL_HPP
#define GODNET_NET_EVENT_LOOP_THREAD_POOL_HPP

#include <vector>
#include <memory>

#include "godnet/util/noncopyable.hpp"
#include "godnet/net/event_loop_thread.hpp"

namespace godnet
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : public Noncopyable
{
public:
    explicit EventLoopThreadPool(std::size_t threadNum) noexcept;

    void start() noexcept;
    void stop() noexcept;

    EventLoop* getNextLoop() noexcept;

    const std::vector<EventLoop*>& getLoops() const noexcept
    {
        return loops_;
    }

private:
    std::vector<std::unique_ptr<EventLoopThread>> loopThreads_{};
    std::vector<EventLoop*> loops_{};
    std::size_t index_{};
};

}

#endif