#include "godnet/net/inet_address.hpp"

#include <cstring>

#include "fmt/core.h"

#include "godnet/util/endian.hpp"

namespace godnet
{

InetAddress::InetAddress(std::uint16_t port,
                         bool loopback,
                         bool ipv6) noexcept
{
    if (ipv6)
    {
        addr_.v6.sin6_family = AF_INET6;
        addr_.v6.sin6_port = hostToNetwork(port);
        addr_.v6.sin6_addr = loopback ? in6addr_loopback : in6addr_any;
    }
    else
    {
        addr_.v4.sin_family = AF_INET;
        addr_.v4.sin_port = hostToNetwork(port);
        addr_.v4.sin_addr.s_addr = loopback ? INADDR_LOOPBACK : INADDR_ANY;
    }
}

InetAddress::InetAddress(std::string_view ip,
                         std::uint16_t port,
                         bool ipv6) noexcept
{
    if (ipv6)
    {
        if (::inet_pton(AF_INET6, ip.data(), &addr_.v6.sin6_addr) > 0)
        {
            addr_.v6.sin6_family = AF_INET6;
            addr_.v6.sin6_port = hostToNetwork(port);
        }
    }
    else
    {
        if (::inet_pton(AF_INET, ip.data(), &addr_.v4.sin_addr) > 0)
        {
            addr_.v4.sin_family = AF_INET;
            addr_.v4.sin_port = hostToNetwork(port);
        }
    }
}

bool InetAddress::isAny() const noexcept
{
    if (isV4())
    {
        return addr_.v4.sin_addr.s_addr == INADDR_ANY;
    }
    return std::memcmp(&addr_.v6.sin6_addr,
                       &in6addr_any,
                       sizeof(addr_.v6.sin6_addr)) == 0;
}

bool InetAddress::isLoopback() const noexcept
{
    if (isV4())
    {
        return addr_.v4.sin_addr.s_addr == INADDR_LOOPBACK;
    }
    return std::memcmp(&addr_.v6.sin6_addr,
                       &in6addr_loopback,
                       sizeof(addr_.v6.sin6_addr)) == 0;
}

std::string InetAddress::toIp() const noexcept
{
    char buffer[INET6_ADDRSTRLEN]{};
    if (isV4())
    {
        ::inet_ntop(family(), &addr_.v4.sin_addr, buffer, sizeof(buffer));
    }
    else
    {
        ::inet_ntop(family(), &addr_.v6.sin6_addr, buffer, sizeof(buffer));
    }
    return buffer;
}

std::uint16_t InetAddress::toPort() const noexcept
{
    if (isV4())
    {
        return networkToHost(addr_.v4.sin_port);
    }
    return networkToHost(addr_.v6.sin6_port);
}

std::string InetAddress::toIpPort() const noexcept
{
    return fmt::format("{}:{}", toIp(), toPort());
}

bool operator==(const InetAddress& localAddr,
                const InetAddress& peerAddr) noexcept
{
    if (&localAddr == &peerAddr)
    {
        return true;
    }
    if (localAddr.isV4() && peerAddr.isV4())
    {
        return localAddr.addr_.v4.sin_addr.s_addr ==
               peerAddr.addr_.v4.sin_addr.s_addr &&
               localAddr.addr_.v4.sin_port == peerAddr.addr_.v4.sin_port;
    }
    else if (localAddr.isV6() && localAddr.isV6())
    {
        return localAddr.addr_.v6.sin6_port == peerAddr.addr_.v6.sin6_port &&
               std::memcmp(&localAddr.addr_.v6.sin6_addr,
                           &peerAddr.addr_.v6.sin6_addr,
                           sizeof(localAddr.addr_.v6.sin6_addr)) == 0;
    }
    return false;
}

}
