#include "godnet/net/event_loop_thread.hpp"

#include <iostream>
#include <thread>

using namespace godnet;

int main()
{
    EventLoopThread thread1;
    thread1.start();
    thread1.getLoop()->runTimerEvery(1s, [](TimerId) {
        std::cout << "你好" << std::endl;
    });

    std::this_thread::sleep_for(50000s);
}