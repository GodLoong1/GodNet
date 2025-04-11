#ifndef GODNET_NETWORK_INET_ADDRESS_HPP
#define GODNET_NETWORK_INET_ADDRESS_HPP

#include "godnet/config.hpp"
#include "godnet/network/socket_types.hpp"

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

    int family() const noexcept
    {
        return data_.base.sa_family;
    }

    bool isV4() const noexcept
    {
        return data_.base.sa_family == AF_INET;
    }

    bool isV6() const noexcept
    {
        return data_.base.sa_family == AF_INET6;
    }

    std::string ip() const;
    std::uint16_t port() const;

private:
    union
    {
        sockaddr_t base;
        sockaddr_in4_t v4;
        sockaddr_in6_t v6;
    } data_;
};

}

#endif
