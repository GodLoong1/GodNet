#include "godnet/network/inet_address.hpp"

#include "godnet/util/debug.hpp"
#include "godnet/util/system.hpp"

#if defined(GODNET_LINUX)
    #include <arpa/inet.h>
#endif

namespace godnet
{

InetAddress::InetAddress()
{
    addr_.v4.sin_family = AF_INET;
    addr_.v4.sin_port = 0;
    addr_.v4.sin_addr.s_addr = INADDR_ANY;
}

InetAddress::InetAddress(std::string_view ip, std::uint16_t port)
{
    if (::inet_pton(AF_INET, ip.data(), &addr_.v4.sin_addr) == 1)
    {
        addr_.v4.sin_family = AF_INET;
        addr_.v4.sin_port = ::htons(port);
    }
    else if (::inet_pton(AF_INET6, ip.data(), &addr_.v6.sin6_addr) == 1)
    {
        addr_.v6.sin6_family = AF_INET6;
        addr_.v6.sin6_port = ::htons(port);
    }
    else
    {
        throw std::system_error(
            std::make_error_code(std::errc::invalid_argument),
                "\n" + system::getStackTrace());
        // GODNET_THROW("invalid ip address");
    }
}

std::string InetAddress::ip() const
{
    if (isV4())
    {
        char buffer[INET_ADDRSTRLEN]{};
        if (::inet_ntop(AF_INET, &addr_.v4.sin_addr, buffer, INET_ADDRSTRLEN) == nullptr)
        {
            GODNET_THROW("invalid ip address");
        }
        return buffer;
    }
    char buffer[INET6_ADDRSTRLEN]{};
    if (::inet_ntop(AF_INET6, &addr_.v6.sin6_addr, buffer, INET6_ADDRSTRLEN) == nullptr)
    {
        GODNET_THROW("invalid ip address");
    }
    return buffer;
}

std::uint16_t InetAddress::port() const
{
    if (isV4())
    {
        return ::ntohs(addr_.v4.sin_port);
    }
    return ::ntohs(addr_.v6.sin6_port);
}

}
