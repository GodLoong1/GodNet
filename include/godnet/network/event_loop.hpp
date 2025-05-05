#ifndef GODNET_NETWORK_EVENT_LOOP_HPP
#define GODNET_NETWORK_EVENT_LOOP_HPP

#include "godnet/config.hpp"

#include <cstdint>
#include <atomic>
#include <ctime>
#include <functional>
#include <memory>

#include "godnet/util/noncopyable.hpp"
#include "godnet/util/lock_free_queue.hpp"
#include "godnet/network/timer.hpp"

namespace godnet
{

class EventPoller;
class EventChannel;
class TimerQueue;

class GODNET_EXPORT EventLoop : Noncopyable
{
public:
    using EventCallback = std::function<void()>;

    static EventLoop* GetLoop() noexcept;

    EventLoop();
    ~EventLoop();

    void start();
    void stop();

    void runInLoop(EventCallback&& func);
    void queueInLoop(EventCallback&& func);

    TimerId runTimer(std::uint64_t delay,
                     std::uint64_t interval,
                     TimerCallback&& callback);
    TimerId runTimerAfter(std::uint64_t delay,
                     TimerCallback&& callback);
    TimerId runTimerEvery(std::uint64_t interval,
                     TimerCallback&& callback);
    void cancelTimer(TimerId id);

    bool isInLoopThread() const noexcept;
    void assertInLoopThread();

    void updateChannel(EventChannel* channel);

private:
    void wakeup();
    void updateTime() noexcept;

    enum class Status : std::uint8_t
    {
        STOP,
        RUNNING,
    };

    // 循环状态
    std::atomic<Status> status_{Status::STOP};
    std::uint64_t threadId_{};
    std::uint64_t steadyTimeMs_{};

    // 定时器事件
    std::unique_ptr<TimerQueue> timers_{};

    // FD事件
    std::unique_ptr<EventPoller> poller_{};
    std::vector<EventChannel*> channels_{};

    // 自定义事件
    int wakeupFds_[2]{-1, -1};
    std::unique_ptr<EventChannel> wakeupChannel_{};
    LockFreeQueue<EventCallback> customEvents_{};
};

}

#endif
