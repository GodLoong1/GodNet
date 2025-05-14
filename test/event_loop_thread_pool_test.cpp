#include "godnet/network/event_loop_thread_pool.hpp"
#include "godnet/network/event_loop_thread.hpp"
#include "godnet/network/event_loop.hpp"

using namespace godnet;

int main()
{
    EventLoopThreadPool threadPool(32, "test");
    threadPool.start();
}