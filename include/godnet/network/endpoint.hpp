#ifndef GODNET_NETWORK_ENDPOINT_HPP
#define GODNET_NETWORK_ENDPOINT_HPP

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

class Endpoint
{
public:
    Endpoint() noexcept;

    Endpoint(std::uint16_t port,
             bool loopback,
             bool ipv6) noexcept;

    Endpoint(std::string_view ip,
             std::uint16_t port,
             bool ipv6);

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

inline Endpoint makeEndpointV4() noexcept
{
    return Endpoint(0, false, false);
}

inline Endpoint makeEndpointV4(std::string_view ip, std::uint16_t port)
{
    return Endpoint(ip, port, false);
}

inline Endpoint makeEndpointV4Any(std::uint16_t port) noexcept
{
    return Endpoint(port, false, false);
}

inline Endpoint makeEndpointV4Loopback(std::uint16_t port) noexcept
{
    return Endpoint(port, true, false);
}

inline Endpoint makeEndpointV6() noexcept
{
    return Endpoint(0, false, true);
}

inline Endpoint makeEndpointV6(std::string_view ip, std::uint16_t port)
{
    return Endpoint(ip, port, true);
}

inline Endpoint makeEndpointV6Any(std::uint16_t port) noexcept
{
    return Endpoint(port, false, true);
}

inline Endpoint makeEndpointV6Loopback(std::uint16_t port) noexcept
{
    return Endpoint(port, true, true);
}

}

#endif
