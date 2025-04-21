#include "godnet/network/socket.hpp"

#include "godnet/util/debug.hpp"
#include "godnet/network/socket_ops.hpp"

namespace godnet
{

Socket::Socket(int sockfd)
    : sockfd_(sockfd)
{
    GODNET_THROW_IF(sockfd_ < 0, "Invalid socket file descriptor");
}

Socket::~Socket()
{
    socket_ops::closeSocket(sockfd_);
}

}