#ifndef GODNET_NET_EVENT_LOOP_THREAD_HPP
#define GODNET_NET_EVENT_LOOP_THREAD_HPP

#include <thread>
#include <atomic>

#include "godnet/util/noncopyable.hpp"

namespace godnet
{

class EventLoop;

class EventLoopThread : Noncopyable
{
public:
    EventLoopThread() noexcept = default;
    ~EventLoopThread();

    void start() noexcept;
    void stop() noexcept;

    EventLoop* getLoop() const noexcept;

private:
    std::thread thread_{};
    std::atomic<EventLoop*> loop_{};
};

}

#endif