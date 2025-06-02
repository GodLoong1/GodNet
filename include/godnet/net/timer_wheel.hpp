#ifndef GODNET_NET_TIMER_WHEEL_HPP
#define GODNET_NET_TIMER_WHEEL_HPP

#include <memory>
#include <functional>
#include <unordered_set>
#include <deque>
#include <vector>

#include "godnet/util/noncopyable.hpp"
#include "godnet/net/timer.hpp"

namespace godnet
{

class EventLoop;

/// 时间轮
class TimerWheel : Noncopyable 
{
public:
    using EntryPtr = std::shared_ptr<void>;
    using EntryBucket = std::unordered_set<EntryPtr>;
    using BucketQueue = std::deque<EntryBucket>;

    TimerWheel(EventLoop* loop,
               TimerDuration maxTimeout,
               TimerDuration ticksInterval = 1ms,
               std::size_t bucketNum = 100) noexcept;
    ~TimerWheel() noexcept;

    void insertEntry(TimerDuration delay, EntryPtr&& entryPtr) noexcept;

    EventLoop* getLoop() const noexcept
    {
        return loop_;
    }

private:
    void insertEntryInLoop(TimerDuration delay, EntryPtr&& entryPtr) noexcept;
    void onTimer() noexcept;

    class DelayEntry : Noncopyable
    {
    public:
        DelayEntry(std::function<void()>&& cb) noexcept
        : cb_(std::move(cb)) { }

        ~DelayEntry()
        {
            cb_();
        }

    private:
        const std::function<void()> cb_;
    };

    EventLoop* loop_;
    TimerDuration ticksInterval_;
    std::size_t bucketNum_;

    std::vector<BucketQueue> wheels_;
    std::size_t tickCounter_{0};
    TimerId timerId_{0};
};

}

#endif