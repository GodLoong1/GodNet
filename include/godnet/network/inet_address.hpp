#ifndef GODNET_NETWORK_INET_ADDRESS_HPP
#define GODNET_NETWORK_INET_ADDRESS_HPP

#include "godnet/config.hpp"

#if defined(GODNET_WIN)
    #include <WinSock2.h>
    #include <ws2tcpip.h>
#elif defined(GODNET_LINUX)
    #include <netinet/in.h>
#endif

#include <string>
#include <string_view>
#include <cstdint>

namespace godnet
{

class GODNET_EXPORT InetAddress
{
public:
    InetAddress();
    InetAddress(std::string_view ip, std::uint16_t port);

    std::uint32_t family() const noexcept
    {
        return static_cast<std::uint32_t>(addr_.base.sa_family);
    }

    bool isV4() const noexcept
    {
        return addr_.base.sa_family == AF_INET;
    }

    bool isV6() const noexcept
    {
        return addr_.base.sa_family == AF_INET6;
    }

    std::string ip() const;
    std::uint16_t port() const;

private:
    union
    {
        sockaddr base;
        sockaddr_in v4;
        sockaddr_in6 v6;
    } addr_{};
};

}

#endif
