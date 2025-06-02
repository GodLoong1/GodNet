#include "godnet/net/timer_wheel.hpp"

#include <cassert>

#include "godnet/net/event_loop.hpp"

namespace godnet
{

TimerWheel::TimerWheel(EventLoop* loop,
                       TimerDuration maxTimeout,
                       TimerDuration ticksInterval,
                       std::size_t bucketNum) noexcept
: loop_(loop),
  ticksInterval_(ticksInterval),
  bucketNum_(bucketNum)
{
    assert(maxTimeout > TimerDuration::zero());
    assert(ticksInterval > TimerDuration::zero());
    assert(maxTimeout > ticksInterval);
    assert(bucketNum > 1);

    // 设置轮数
    std::size_t maxBucketNum = maxTimeout / ticksInterval_;
    std::size_t wheelsNum = 1;
    while (maxBucketNum > bucketNum)
    {
        ++wheelsNum;
        bucketNum *= bucketNum_;
    }
    wheels_.resize(wheelsNum);

    // 设置桶数
    for (BucketQueue& bucket : wheels_)
    {
        bucket.resize(bucketNum_);
    }

    timerId_ = loop_->runTimerEvery(ticksInterval_, [this] {
        ++tickCounter_;
        size_t pow = 1;

        for (BucketQueue& bucket : wheels_)
        {
            if ((tickCounter_ % pow) == 0)
            {
                bucket.pop_front();
                bucket.emplace_back();
            }
            pow *= bucketNum_;
        }
    });
}

TimerWheel::~TimerWheel() noexcept
{
    loop_->assertInLoopThread();

    loop_->cancelTimer(timerId_);
    for (auto it = wheels_.rbegin(); it != wheels_.rend(); ++it)
    {
        it->clear();
    }
}

void TimerWheel::insertEntry(TimerDuration delay, EntryPtr&& entryPtr) noexcept
{
    if (loop_->isInLoopThread())
    {
        insertEntryInLoop(delay, std::move(entryPtr));
    }
    else
    {
        loop_->queueInLoop(
            [this, delay, entryPtr(std::move(entryPtr))] () mutable {
                insertEntryInLoop(delay, std::move(entryPtr));
        });
    }
}

void TimerWheel::insertEntryInLoop(TimerDuration delay, EntryPtr&& entryPtr) noexcept
{
    loop_->assertInLoopThread();

    std::size_t index = static_cast<size_t>(delay / ticksInterval_ + 1);
    std::size_t t = tickCounter_;

    for (std::size_t i = 0; i != wheels_.size(); ++i)
    {
        // 在第一个时间轮
        if (index <= bucketNum_)
        {
            wheels_[i][index - 1].insert(std::move(entryPtr));
            break;
        }
        // 不是最后一个时间轮
        if (i != (wheels_.size() - 1))
        {
            entryPtr = std::make_shared<DelayEntry>(
                [this, index, i, t, entryPtr(std::move(entryPtr))] {
                    // 计算上一层的位置
                    wheels_[i][(index + (t % bucketNum_) - 1) %
                               bucketNum_].insert(std::move(entryPtr));
            });
        }
        else
        {
            // 插入最后一个桶
            wheels_[i][bucketNum_ - 1].insert(std::move(entryPtr));
        }

        // 计算下一个轮间隔
        index = (index + (t % bucketNum_) - 1) / bucketNum_;
        t /= bucketNum_;
    }
}

}