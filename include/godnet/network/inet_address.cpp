#include "godnet/network/inet_address.hpp"

#include "fmt/format.h"

#include "godnet/util/debug.hpp"

namespace godnet
{

InetAddress InetAddress::MakeV4(std::string_view ip, std::uint16_t port)
{
    return InetAddress(ip, port, false);
}

InetAddress InetAddress::MakeV4Any(std::uint16_t port) noexcept
{
    return InetAddress(port, false, false);
}

InetAddress InetAddress::MakeV4Loopback(std::uint16_t port) noexcept
{
    return InetAddress(port, true, false);
}

InetAddress InetAddress::MakeV6(std::string_view ip, std::uint16_t port)
{
    return InetAddress(ip, port, true);
}

InetAddress InetAddress::MakeV6Any(std::uint16_t port) noexcept
{
    return InetAddress(port, false, true);
}

InetAddress InetAddress::MakeV6Loopback(std::uint16_t port) noexcept
{
    return InetAddress(port, true, true);
}

InetAddress::InetAddress(std::uint16_t port,
                         bool loopback,
                         bool ipv6) noexcept
{
    if (ipv6)
    {
        addr_.v6.sin6_family = AF_INET6;
        addr_.v6.sin6_port = ::htons(port);
        addr_.v6.sin6_addr = loopback ? in6addr_loopback : in6addr_any;
    }
    else
    {
        addr_.v4.sin_family = AF_INET;
        addr_.v4.sin_port = ::htons(port);
        addr_.v4.sin_addr.s_addr = loopback ? INADDR_LOOPBACK : INADDR_ANY;
    }
}

InetAddress::InetAddress(std::string_view ip,
                         std::uint16_t port,
                         bool ipv6)
{
    if (ipv6)
    {
        if (::inet_pton(AF_INET6, ip.data(), &addr_.v6.sin6_addr) > 0)
        {
            addr_.v6.sin6_family = AF_INET6;
            addr_.v6.sin6_port = ::htons(port);
            return;
        }
    }
    else
    {
        if (::inet_pton(AF_INET, ip.data(), &addr_.v4.sin_addr) > 0)
        {
            addr_.v4.sin_family = AF_INET;
            addr_.v4.sin_port = ::htons(port);
            return;
        }
    }
    GODNET_THROW_RUNERR("invalid ip address");
}

std::string InetAddress::toIp() const noexcept
{
    char buffer[INET6_ADDRSTRLEN]{};
    if (isV4())
    {
        ::inet_ntop(AF_INET, &addr_.v4.sin_addr, buffer, sizeof(buffer));
    }
    else
    {
        ::inet_ntop(AF_INET6, &addr_.v6.sin6_addr, buffer, sizeof(buffer));
    }
    return buffer;
}

std::uint16_t InetAddress::toPort() const noexcept
{
    if (isV4())
    {
        return ::ntohs(addr_.v4.sin_port);
    }
    return ::ntohs(addr_.v6.sin6_port);
}

std::string InetAddress::toIpPort() const noexcept
{
    return fmt::format("{}:{}", toIp(), toPort());
}

}
