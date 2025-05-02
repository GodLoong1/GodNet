#ifndef GODNET_NETWORK_EVENT_LOOP_HPP
#define GODNET_NETWORK_EVENT_LOOP_HPP

#include "godnet/config.hpp"
#include "godnet/util/noncopyable.hpp"
#include "godnet/util/lock_free_queue.hpp"

#include <cstdint>
#include <atomic>
#include <memory>
#include <functional>
#include <type_traits>
#include <queue>

namespace godnet
{

class EventPoller;
class EventChannel;

class GODNET_EXPORT EventLoop : Noncopyable
{
public:
    using EventCallback = std::function<void()>;

    EventLoop();
    ~EventLoop();

    void start();
    void stop();

    void runInLoop(EventCallback&& func);
    void queueInLoop(EventCallback&& func);

    bool isInLoopThread() const noexcept;
    void assertInLoopThread();

    void updateChannel(EventChannel* channel);

private:
    void wakeup();

    enum class Status : std::uint8_t
    {
        STOP,
        RUNNING,
    };

    std::atomic<Status> status_{Status::STOP};
    std::uint64_t threadId_{};
    std::uint64_t startTimeMs_{};
    std::uint64_t startTimeUs_{};
    std::uint64_t curTimeUs_{};

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
