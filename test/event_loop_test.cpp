#include "godnet/network/event_loop.hpp"
#include <iostream>

int main()
{
    godnet::EventLoop loop;
    loop.queueInLoop([]{
        std::cout << "Hello, World!" << std::endl;
    });
    loop.queueInLoop([]{
        std::cout << "Hello, World!" << std::endl;
    });
    loop.queueInLoop([]{
        std::cout << "Hello, World!" << std::endl;
    });
    loop.queueInLoop([]{
        std::cout << "Hello, World!" << std::endl;
    });
    loop.queueInLoop([]{
        std::cout << "Hello, World!" << std::endl;
    });
    loop.loop();
}