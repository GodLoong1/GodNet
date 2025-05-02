#include "godnet/network/event_loop.hpp"
#include "godnet/util/debug.hpp"
#include "godnet/util/system.hpp"
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
    loop.start();
}