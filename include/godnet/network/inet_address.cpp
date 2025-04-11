#include "godnet/network/inet_address.hpp"

#include "godnet/debug.hpp"

namespace godnet
{

InetAddress::InetAddress()
: data_()
{
    data_.v4.sin_family = AF_INET;
    data_.v4.sin_port = 0;
    data_.v4.sin_addr.s_addr = INADDR_ANY;
}

InetAddress::InetAddress(std::string_view ip, std::uint16_t port)
: data_()
{
    if (::inet_pton(AF_INET, ip.data(), &data_.v4.sin_addr) == 1)
    {
        data_.v4.sin_family = AF_INET;
        data_.v4.sin_port = htons(port);
    }
    else if (::inet_pton(AF_INET6, ip.data(), &data_.v6.sin6_addr) == 1)
    {
        data_.v6.sin6_family = AF_INET6;
        data_.v6.sin6_port = htons(port);
    }
    else
    {
        GODNET_THROW("invalid ip address");
    }
}

std::string InetAddress::ip() const
{
    if (isV4())
    {
        char buffer[INET_ADDRSTRLEN]{};
        if (::inet_ntop(AF_INET, &data_.v4.sin_addr, buffer, INET_ADDRSTRLEN) == nullptr)
        {
            GODNET_THROW("invalid ip address");
        }
        return buffer;
    }
    char buffer[INET6_ADDRSTRLEN]{};
    if (::inet_ntop(AF_INET6, &data_.v6.sin6_addr, buffer, INET6_ADDRSTRLEN) == nullptr)
    {
        GODNET_THROW("invalid ip address");
    }
    return buffer;
}

std::uint16_t InetAddress::port() const
{
    if (isV4())
    {
        return ::ntohs(data_.v4.sin_port);
    }
    return ::ntohs(data_.v6.sin6_port);
}

}
