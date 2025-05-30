#include "godnet/net/timer_wheel.hpp"

#include <cassert>
#include <chrono>

namespace godnet
{

TimerWheel::TimerWheel(EventLoop* loop,
                       size_t maxTimeout,
                       std::chrono::milliseconds ticksInterval,
                       size_t bucketNum) noexcept
: loop_(loop),
  ticksInterval_(ticksInterval),
  bucketNum_(bucketNum)
{
    assert(maxTimeout > 1);
    assert(ticksInterval > 0ms);
    assert(bucketNum > 1);

    // 设置轮数
    size_t maxBucketNum = maxTimeout / ticksInterval_.count();
    size_t wheelsNum = 1;
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
        onTimer();
    });

    LOG_TRACE << getThreadName() << ": maxTimeout: "
              << maxTimeout << ", interval: " << ticksInterval_
              << ", wheelsNum: " << wheelsNum << ", bucketNum: "
              << bucketNum_;
}

TimerWheel::~TimerWheel() noexcept
{
    loop_->assertInLoop();

    loop_->cancelTimer(timerId_);
    for (auto it = wheels_.rbegin(); it != wheels_.rend(); ++it)
    {
        it->clear();
    }

    LOG_TRACE << getThreadName();
}

void TimerWheel::insertEntry(size_t delay, EntryPtr&& entryPtr) noexcept
{
    if (loop_->isInLoop())
    {
        insertEntryInLoop(delay, std::move(entryPtr));
    }
    else
    {
        loop_->addInLoop(
            [this, delay, entryPtr(std::move(entryPtr))] () mutable {
                insertEntryInLoop(delay, std::move(entryPtr));
        });
    }
}

void TimerWheel::insertEntryInLoop(size_t delay, EntryPtr&& entryPtr) noexcept
{
    loop_->assertInLoop();

    delay = static_cast<size_t>(delay / ticksInterval_ + 1);
    size_t t = tickCounter_;

    for (size_t i = 0; i != wheels_.size(); ++i)
    {
        // 在第一个时间轮
        if (delay <= bucketNum_)
        {
            wheels_[i][delay - 1].insert(std::move(entryPtr));
            break;
        }
        // 不是最后一个时间轮
        if (i != (wheels_.size() - 1))
        {
            entryPtr = std::make_shared<DelayEntry>(
                [this, delay, i, t, entryPtr(std::move(entryPtr))] {
                    // 计算上一层的位置
                    wheels_[i][(delay + (t % bucketNum_) - 1) %
                               bucketNum_].insert(std::move(entryPtr));
            });
        }
        else
        {
            // 插入最后一个桶
            wheels_[i][bucketNum_ - 1].insert(std::move(entryPtr));
        }

        // 计算下一个轮间隔
        delay = (delay + (t % bucketNum_) - 1) / bucketNum_;
        t /= bucketNum_;
    }
}

void TimerWheel::onTimer() noexcept
{
    loop_->assertInLoop();

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
}

}