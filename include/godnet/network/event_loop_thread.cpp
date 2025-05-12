#include "godnet/network/event_loop_thread.hpp"

#ifdef __linux__
    #include <sys/prctl.h>
#endif

#include <cassert>

#ifndef NDEBUG
    #include "godnet/util/system.hpp"
#endif

namespace godnet
{

EventLoopThread::~EventLoopThread()
{
    stop();
}

void EventLoopThread::start() noexcept
{
    assert(!thread_.joinable());
    thread_ = std::thread([this] {
#ifdef __linux__
        ::prctl(PR_SET_NAME, name_.c_str());
#endif
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
    if (loopPtr)
    {
        assert(loopPtr->threadId() != system::getThreadId());
        loopPtr->stop();
    }
    if (thread_.joinable())
    {
        thread_.join();
    }
}

EventLoop* EventLoopThread::getLoop() noexcept
{
    assert(loop_);
    return loop_.load(std::memory_order_acquire);
}

}