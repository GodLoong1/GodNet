#ifndef GODNET_NETWORK_EVENT_LOOP_HPP
#define GODNET_NETWORK_EVENT_LOOP_HPP

#include "godnet/config.hpp"
#include "godnet/util/noncopyable.hpp"
#include "godnet/util/lock_free_queue.hpp"
#include "godnet/network/event_base.hpp"

#include <cstdint>
#include <atomic>
#include <memory>
#include <functional>
#include <type_traits>
#include <queue>

namespace godnet
{

class EventPoller;
class ChannelEvent;

class GODNET_EXPORT EventLoop : Noncopyable
{
public:
    using EventCallback = std::function<void()>;

    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    SignalEventId addSiganlEvent(EventCallback&& callback, int signo);
    bool delSignalEvent(SignalEventId eventId);

    template <typename Func, typename =
        std::enable_if_t<std::is_constructible_v<EventCallback, Func>>>
    void runInLoop(Func&& func)
    {
        if (isInLoop())
        {
            func();
        }
        else
        {
            queueInLoop(std::forward<Func>(func));
        }
    }

    void queueInLoop(const EventCallback& func);
    void queueInLoop(EventCallback&& func);

    bool isInLoop() const noexcept;
    void assertInLoop();

    void updateChannel(ChannelEvent* channel);

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

    std::vector<SignalEvent*> signalEvents_{};
    std::unordered_map<SignalEventId, SignalEvent*> signalEventIds_{};

    std::unique_ptr<EventPoller> poller_{};
    std::vector<ChannelEvent*> ready_channel_vec_{};
    LockFreeQueue<EventCallback> event_callback_queue_{};
    std::priority_queue<ChannelEvent*> timer_queue_{};

    // 自定义事件
    int wakeupFds_[2];
    std::unique_ptr<ChannelEvent> wakeupChannel_;
    LockFreeQueue<CustomEvent> customEvents_{};
};

}

#endif
