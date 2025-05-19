#include "godnet/net/event_loop_thread_pool.hpp"

#include <cassert>

#include "godnet/net/event_loop_thread.hpp"

namespace godnet
{

EventLoopThreadPool::EventLoopThreadPool(std::size_t threadNum) noexcept
{
    loopThreads_.reserve(threadNum);
    for (std::size_t i = 0; i != threadNum; ++i)
    {
        loopThreads_.push_back(std::make_unique<EventLoopThread>());
    }
}

EventLoopThreadPool::~EventLoopThreadPool()
{
    assert(loops_.empty());
}

void EventLoopThreadPool::start() noexcept
{
    assert(loops_.empty());
    for (const std::unique_ptr<EventLoopThread>& loopPtr : loopThreads_)
    {
        loopPtr->start();
    }

    loops_.reserve(loops_.size());
    for (const std::unique_ptr<EventLoopThread>& loopPtr : loopThreads_)
    {
        loops_.emplace_back(loopPtr->getLoop());
    }
}

void EventLoopThreadPool::stop() noexcept
{
    assert(!loops_.empty());
    for (const std::unique_ptr<EventLoopThread>& loopPtr : loopThreads_)
    {
        loopPtr->stop();
    }
    loops_.clear();
}

EventLoop* EventLoopThreadPool::getNextLoop() noexcept
{
    assert(!loops_.empty());
    if (index_ >= loops_.size())
    {
        index_ = 0;
    }
    return loops_[index_++];
}

}