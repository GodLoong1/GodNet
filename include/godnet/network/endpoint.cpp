#include "godnet/network/endpoint.hpp"

#include <stdexcept>

#include "fmt/core.h"

#include "godnet/util/exception.hpp"

namespace godnet
{

Endpoint::Endpoint(std::uint16_t port,
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

Endpoint::Endpoint(std::string_view ip,
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
    throwGodnetRunErr("invalid ip: {}", ip);
}

std::string Endpoint::toIp() const noexcept
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

std::uint16_t Endpoint::toPort() const noexcept
{
    if (isV4())
    {
        return ::ntohs(addr_.v4.sin_port);
    }
    return ::ntohs(addr_.v6.sin6_port);
}

std::string Endpoint::toIpPort() const noexcept
{
    return fmt::format("{}:{}", toIp(), toPort());
}

}
