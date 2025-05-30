#include "godnet/net/event_loop_thread.hpp"

#include <cassert>
#include <future>

#include "godnet/util/system.hpp"

namespace godnet
{

EventLoopThread::~EventLoopThread()
{
    if (thread_.joinable())
    {
        stop();
    }
}

void EventLoopThread::start() noexcept
{
    assert(!thread_.joinable());

    std::promise<void> promise;
    std::future<void> future = promise.get_future();

    thread_ = std::thread([this, &promise] {
        EventLoop loop;
        loop.queueInLoop([this, &loop, &promise]{
            loop_.store(&loop, std::memory_order_release);
            promise.set_value();
        });

        loop.start();
        loop_.store(nullptr, std::memory_order_release);
    });

    future.get();
}

void EventLoopThread::stop() noexcept
{
    EventLoop* loopPtr = loop_.load(std::memory_order_acquire);
    assert(loopPtr);
    assert(loopPtr->threadId() != system::getThreadId());
    loopPtr->stop();

    assert(thread_.joinable());
    thread_.join();
}

EventLoop* EventLoopThread::getLoop() const noexcept
{
    assert(loop_);
    return loop_.load(std::memory_order_acquire);
}

}