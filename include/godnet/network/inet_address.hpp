#ifndef GODNET_NETWORK_INET_ADDRESS_HPP
#define GODNET_NETWORK_INET_ADDRESS_HPP

#include "godnet/config.hpp"

#include <string>
#include <string_view>
#include <cstdint>

#if defined(GODNET_WIN)
    #include <WinSock2.h>
    #include <ws2tcpip.h>
#elif defined(GODNET_LINUX)
    #include <arpa/inet.h>
#endif

namespace godnet
{

class GODNET_EXPORT InetAddress
{
public:
    InetAddress();
    InetAddress(std::string_view ip, std::uint16_t port);

    int family() const noexcept
    {
        return addr_.base.sa_family;
    }

    bool isV4() const noexcept
    {
        return addr_.base.sa_family == AF_INET;
    }

    bool isV6() const noexcept
    {
        return addr_.base.sa_family == AF_INET6;
    }

    const struct sockaddr* getSockAddr() const noexcept
    {
        return &addr_.base;
    }

    struct sockaddr* mutSockAddr() noexcept
    {
        return &addr_.base;
    }

    std::size_t getSockLen() const noexcept 
    {
        return isV4() ? sizeof(addr_.v4) : sizeof(addr_.v6);
    }

    std::string toIp() const noexcept;
    std::uint16_t toPort() const noexcept;
    std::string toIpPort() const noexcept;

private:
    union
    {
        struct sockaddr base;
        struct sockaddr_in v4;
        struct sockaddr_in6 v6;
    } addr_{};
};

}

#endif
