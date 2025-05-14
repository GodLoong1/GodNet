#ifndef GODNET_NETWORK_EVENT_LOOP_THREAD_HPP
#define GODNET_NETWORK_EVENT_LOOP_THREAD_HPP

#include <thread>
#include <atomic>
#include <string>

#include "godnet/util/noncopyable.hpp"

namespace godnet
{

class EventLoop;

class EventLoopThread : Noncopyable
{
public:
    EventLoopThread(std::string threadName) noexcept
    : name_(std::move(threadName)) {}

    ~EventLoopThread();

    void start() noexcept;
    void stop() noexcept;
    EventLoop* getLoop() noexcept;

private:
    std::string name_{};
    std::thread thread_{};
    std::atomic<EventLoop*> loop_{};
};

}

#endif
