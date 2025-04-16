#include "godnet/network/socket.hpp"

#include "godnet/util/debug.hpp"

namespace godnet
{

Socket::Socket(int sockfd)
    : sockfd_(sockfd)
{
    GODNET_THROW_IF(sockfd_ < 0, "Invalid socket file descriptor");
}

}