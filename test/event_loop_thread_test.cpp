#include "godnet/net/event_loop.hpp"
#include "godnet/net/event_loop_thread.hpp"
#include "godnet/util/logger.hpp"

using namespace godnet;

int main()
{
    EventLoopThread thread1;
    thread1.start();
    thread1.getLoop()->runTimerEvery(999ms, [count(0)]() mutable {
        GODNET_LOG_INFO("count: {}", ++count);
    });
    std::this_thread::sleep_for(3s);
    thread1.stop();
    thread1.start();
    thread1.getLoop()->runTimerEvery(499ms, []{
        GODNET_LOG_INFO("restart !");
    });
    std::this_thread::sleep_for(2s);
}