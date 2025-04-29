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
class ChannelEvent;

class GODNET_EXPORT EventLoop : Noncopyable
{
public:
    using EventCallback = std::function<void()>;

    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

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

    std::atomic<bool> looping_{};
    std::atomic<bool> quit_{};
    std::uint64_t thread_id_{};

    std::unique_ptr<EventPoller> poller_{};
    std::vector<ChannelEvent*> ready_channel_vec_{};
    LockFreeQueue<EventCallback> event_callback_queue_{};
    std::priority_queue<ChannelEvent*> timer_queue_{};

    int wakeup_fd_{};
    std::unique_ptr<ChannelEvent> wakeup_channel_{};
};

}

#endif
