#include "godnet/network/event_loop_thread_pool.hpp"

using namespace godnet;

int main()
{
    EventLoopThreadPool threadPool(32, "test");
    threadPool.start();
}