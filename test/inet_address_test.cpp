#include "godnet/network/inet_address.hpp"
#include <iostream>
#include "godnet/exception.hpp"

int main()
{
    godnet::InetAddress inet_address;
    std::cout << inet_address.ipPort() << std::endl;

    godnet::InetAddress inet_address1("127.127.127.127", 80);
    std::cout << inet_address1.ipPort() << std::endl;

    godnet::InetAddress inet_address2("2a03:2880:f12f:83:face:b00c:0:25de", 80);
    std::cout << inet_address2.ipPort() << std::endl;

    try
    {
        godnet::InetAddress inet_address3("fadfasdffdafds", 89);
    }
    catch (godnet::GodNetException& e)
    {
        std::cout << e.what() << std::endl;
    }
}