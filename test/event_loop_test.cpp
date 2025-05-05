#include <iostream>

#include "godnet/network/event_loop.hpp"

using namespace godnet;

int main()
{
    godnet::EventLoop loop;
    loop.runInLoop([]{
        std::cout << "call runInLoop" << std::endl;
    });
    loop.queueInLoop([]{
        std::cout << "call queueInLoop" << std::endl;
    });
    loop.runTimer(1000, 500, []{
        std::cout << "call runTimer(1000, 500)" << std::endl;
    });
    loop.runTimerAfter(2000, []{
        std::cout << "call  runTimerAfter(2000)" << std::endl;
    });
    loop.runTimerEvery(700, []{
        std::cout << "call runTimerEvery(700)" << std::endl;
    });
    loop.start();
}