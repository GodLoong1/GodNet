#ifndef GODNET_NETWORK_SOCKET_OPS_HPP
#define GODNET_NETWORK_SOCKET_OPS_HPP

#include "godnet/config.hpp"

namespace godnet
{

class InetAddress;

namespace socket_ops
{

void setNonBlock(int sockfd);

int createTcpSocket(int family);

int closeSocket(int sockfd);

int bindAddress(int sockfd, const InetAddress& addr);

int listenSocket(int sockfd);

int acceptSocket(int sockfd, InetAddress& addr);

int closeWrite(int sockfd);

int setTcpNoDelay(int sockfd, bool on);

int setReuseAddr(int sockfd, bool on);

int setReusePort(int sockfd, bool on);

int setKeepAlive(int sockfd, bool on);

int getSocketError(int sockfd);

}
}

#endif
