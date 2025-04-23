#include "godnet/network/inet_address.hpp"
#include "fmt/format.h"

int main()
{
    godnet::InetAddress addr1("1.15.234.12", 2024);
    fmt::print("addr1: {}\n", addr1.toIpPort());

    godnet::InetAddress addr2("fe80::5054:ff:feda:eec9", 1024);
    fmt::print("addr2: {}\n", addr2.toIpPort());

    try
    {
        godnet::InetAddress addr("fffasfds", 80);
    }
    catch (const std::exception &e)
    {
        fmt::print("Exception: {}\n", e.what());
    }
}