#ifndef GODNET_NETWORK_EVENT_LOOP_HPP
#define GODNET_NETWORK_EVENT_LOOP_HPP

#include "godnet/config.hpp"

namespace godnet
{

class EventChannel;

class GODNET_EXPORT EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void runInLoop();
    void queueInLoop();

    void assertInLoop();

    void updateChannel(EventChannel* channel);

private:

};

}

#endif
