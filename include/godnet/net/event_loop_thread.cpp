#include "godnet/net/event_loop_thread.hpp"

#include <cassert>

#include "godnet/util/system.hpp"
#include "godnet/net/event_loop.hpp"

namespace godnet
{

EventLoopThread::~EventLoopThread()
{
    assert(!thread_.joinable());
}

void EventLoopThread::start() noexcept
{
    assert(!thread_.joinable());
    thread_ = std::thread([this] {
        EventLoop loop;
        loop.queueInLoop([this, &loop]{
            loop_.store(&loop, std::memory_order_release);
        });

        loop.start();
        loop_.store(nullptr, std::memory_order_release);
    });

    while (!loop_.load(std::memory_order_acquire))
    {
        std::this_thread::yield();
    }
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