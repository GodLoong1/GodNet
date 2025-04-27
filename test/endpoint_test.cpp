#include "godnet/network/endpoint.hpp"
#include "fmt/core.h"

int main()
{
    auto addr1{godnet::makeEndpointV4("1.15.234.12", 2024)};
    auto addr2{godnet::makeEndpointV4Any(2024)};
    auto addr3{godnet::makeEndpointV4Loopback(2024)};
    auto addr4{godnet::makeEndpointV6("fe80::5054:ff:feda:eec9", 1024)};
    auto addr5{godnet::makeEndpointV6Any(1024)};
    auto addr6{godnet::makeEndpointV6Loopback(1024)};
    auto addr7{godnet::makeEndpointV4()};
    auto addr8{godnet::makeEndpointV6()};

    fmt::print("addr1: {}\n", addr1.toIpPort());
    fmt::print("addr2: {}\n", addr2.toIpPort());
    fmt::print("addr3: {}\n", addr3.toIpPort());
    fmt::print("addr4: {}\n", addr4.toIpPort());
    fmt::print("addr5: {}\n", addr5.toIpPort());
    fmt::print("addr6: {}\n", addr6.toIpPort());
    fmt::print("addr7: {}\n", addr7.toIpPort());
    fmt::print("addr8: {}\n", addr8.toIpPort());

    try
    {
        godnet::makeEndpointV4("fdsf,fadfas,fdafs", 8080);
    }
    catch (std::exception& e1)
    {
        fmt::print("Exception caught: {}\n", e1.what());
        try
        {
            godnet::makeEndpointV6("fdasf,fdasf,fdasf", 8080);
        }
        catch (std::exception& e2)
        {
            fmt::print("Exception caught: {}\n", e2.what());
        }
    }
}