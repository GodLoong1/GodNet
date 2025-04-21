#ifndef GODNET_NETWORK_SOCKET_HPP
#define GODNET_NETWORK_SOCKET_HPP

#include "godnet/config.hpp"
#include "godnet/util/noncopyable.hpp"

namespace godnet
{

class Socket : Noncopyable
{
public:
    explicit Socket(int sockfd);
    ~Socket();

private:
    int sockfd_;
};

}

#endif
