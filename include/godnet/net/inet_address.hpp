#ifndef GODNET_NET_INET_ADDRESS_HPP
#define GODNET_NET_INET_ADDRESS_HPP

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
#endif

#include <string>
#include <string_view>
#include <cstdint>

namespace godnet
{

class InetAddress
{
public:
    static InetAddress V4() noexcept
    {
        return InetAddress(0, false, false);
    }

    static InetAddress V4(std::string_view ip, std::uint16_t port) noexcept
    {
        return InetAddress(ip, port, false);
    }

    static InetAddress V4Any(std::uint16_t port) noexcept
    {
        return InetAddress(port, false, false);
    }

    static InetAddress V4Loopback(std::uint16_t port) noexcept
    {
        return InetAddress(port, true, false);
    }

    static InetAddress V6() noexcept
    {
        return InetAddress(0, false, true);
    }

    static InetAddress V6(std::string_view ip, std::uint16_t port) noexcept
    {
        return InetAddress(ip, port, true);
    }

    static InetAddress V6Any(std::uint16_t port) noexcept
    {
        return InetAddress(port, false, true);
    }

    static InetAddress V6Loopback(std::uint16_t port) noexcept
    {
        return InetAddress(port, true, true);
    }

public:
    InetAddress() noexcept = default;

    InetAddress(std::uint16_t port,
                bool loopback,
                bool ipv6) noexcept;

    InetAddress(std::string_view ip,
                std::uint16_t port,
                bool ipv6) noexcept;

    int family() const noexcept
    {
        return addr_.base.sa_family;
    }

    bool isV4() const noexcept
    {
        return family() == AF_INET;
    }

    bool isV6() const noexcept
    {
        return family() == AF_INET6;
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

    bool valid() const noexcept
    {
        if (family() != AF_INET || family() != AF_INET6)
        {
            return true;
        }
        return false;
    }

    bool isAny() const noexcept;
    bool isLoopback() const noexcept;

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
