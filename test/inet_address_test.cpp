#include "godnet/network/inet_address.hpp"
#include "fmt/format.h"

int main()
{
    auto addr1{godnet::InetAddress::MakeV4("1.15.234.12", 2024)};
    auto addr2{godnet::InetAddress::MakeV4Any(2024)};
    auto addr3{godnet::InetAddress::MakeV4Loopback(2024)};
    auto addr4{godnet::InetAddress::MakeV6("fe80::5054:ff:feda:eec9", 1024)};
    auto addr5{godnet::InetAddress::MakeV6Any(1024)};
    auto addr6{godnet::InetAddress::MakeV6Loopback(1024)};

    fmt::print("addr1: {}\n", addr1.toIpPort());
    fmt::print("addr2: {}\n", addr2.toIpPort());
    fmt::print("addr3: {}\n", addr3.toIpPort());
    fmt::print("addr4: {}\n", addr4.toIpPort());
    fmt::print("addr5: {}\n", addr5.toIpPort());
    fmt::print("addr6: {}\n", addr6.toIpPort());

    try
    {
        godnet::InetAddress::MakeV4("fdsf,fadfas,fdafs", 8080);
    }
    catch (std::exception& e1)
    {
        fmt::print("Exception caught: {}\n", e1.what());
        try
        {
            godnet::InetAddress::MakeV6("fdasf,fdasf,fdasf", 8080);
        }
        catch (std::exception& e2)
        {
            fmt::print("Exception caught: {}\n", e2.what());
        }
    }
}