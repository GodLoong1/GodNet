#include "godnet/network/event_loop_thread.hpp"
#include "godnet/network/event_loop.hpp"
#include "fmt/core.h"
#include <chrono>
#include <thread>

using namespace godnet;

int main()
{
    EventLoopThread thread{"test1"};
    thread.start();
    thread.getLoop()->runTimerEvery(1000, []{
        fmt::println("hello");
    });
    std::this_thread::sleep_for(std::chrono::seconds(5));
    thread.stop();
}