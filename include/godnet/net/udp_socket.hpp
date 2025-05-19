#ifndef GODNET_NETWORK_UDP_SOCKET_HPP
#define GODNET_NETWORK_UDP_SOCKET_HPP

#include "godnet/config.hpp"

namespace godnet
{

class UdpSocket
{
public:
    UdpSocket(int sockfd);
    ~UdpSocket();

private:
    int sockfd_;
};

}

#endif
