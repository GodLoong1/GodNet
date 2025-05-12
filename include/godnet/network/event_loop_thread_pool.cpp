#include "godnet/network/event_loop_thread_pool.hpp"

namespace godnet
{

EventLoopThreadPool::EventLoopThreadPool(std::uint32_t threadNum,
                                         std::string threadName) noexcept
{
    loops_.reserve(threadNum);
    for (std::uint32_t i = 0; i != threadNum; ++i)
    {
        loops_.push_back(std::make_unique<EventLoopThread>(threadName));
    }
}

void EventLoopThreadPool::start() noexcept
{
    for (const auto& loop : loops_)
    {
        loop->start();
    }
}

void EventLoopThreadPool::stop() noexcept
{
    for (const auto& loop : loops_)
    {
        loop->stop();
    }
}

std::vector<EventLoop*> EventLoopThreadPool::getLoops() noexcept
{
    std::vector<EventLoop*> ret;
    ret.reserve(loops_.size());

    for (const auto& loop : loops_)
    {
        ret.emplace_back(loop->getLoop());
    }
    return ret;
}

}