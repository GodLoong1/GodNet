#ifndef GODNET_NET_EVENT_LOOP_HPP
#define GODNET_NET_EVENT_LOOP_HPP

#include <cstdint>
#include <atomic>
#include <functional>
#include <memory>

#include "godnet/util/noncopyable.hpp"
#include "godnet/util/lock_free_queue.hpp"
#include "godnet/net/timer.hpp"

namespace godnet
{

class EventPoller;
class EventChannel;
class TimerQueue;

class EventLoop : Noncopyable
{
public:
    using EventCallback = std::function<void()>;

    static EventLoop* GetLoop() noexcept;

    EventLoop() noexcept;
    ~EventLoop();

    void start();
    void stop() noexcept;

    void runInLoop(EventCallback&& func);
    void queueInLoop(EventCallback&& func) noexcept;

    TimerId runTimer(TimerDuration delay,
                     TimerDuration interval,
                     TimerCallback&& callback) noexcept;
    TimerId runTimerAfter(TimerDuration delay,
                          TimerCallback&& callback) noexcept;
    TimerId runTimerEvery(TimerDuration interval,
                          TimerCallback&& callback) noexcept;
    void cancelTimer(TimerId id) noexcept;

    bool isInLoopThread() const noexcept;
    void assertInLoopThread() const noexcept;

    void updateChannel(EventChannel* channel) noexcept;

    bool isStop() const noexcept
    {
        return status_.load(std::memory_order_acquire) == Status::stop;
    }

    bool isRunning() const noexcept
    {
        return status_.load(std::memory_order_acquire) == Status::running;
    }

    std::uint64_t threadId() const noexcept
    {
        return threadId_;
    }

private:
    void wakeup();

    enum class Status : std::uint8_t
    {
        stop,
        running,
    };

    std::uint64_t threadId_;
    std::atomic<Status> status_{Status::stop};

    std::unique_ptr<TimerQueue> timers_;

    std::unique_ptr<EventPoller> poller_;
    std::vector<EventChannel*> channels_;

    LockFreeQueue<EventCallback> customEvents_;

#ifdef __linux__
    int wakeupFd_{-1};
    std::unique_ptr<EventChannel> wakeupChannel_;
#endif
};

}

#endif
