#ifndef GODNET_NETWORK_EVENT_LOOP_HPP
#define GODNET_NETWORK_EVENT_LOOP_HPP

#include "godnet/config.hpp"
#include "godnet/util/noncopyable.hpp"
#include "godnet/util/lock_free_queue.hpp"

#include <atomic>
#include <memory>
#include <functional>

namespace godnet
{

class EventChannel;

class GODNET_EXPORT EventLoop : Noncopyable
{
public:
    using EventCallback = std::function<void()>;

    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void runInLoop();
    void queueInLoop();

    void assertInLoop();

    void updateChannel(EventChannel* channel);

private:
    std::atomic<bool> looping_;
    std::atomic<bool> quit_;
    LockFreeQueue<EventCallback> event_callback_queue_;

#if defined(GODNET_LINUX)
    int wakeup_fd_;
    std::unique_ptr<EventChannel> wakeup_channel_;
#endif
};

}

#endif
