#ifndef GODNET_NETWORK_SOCKET_OPS_HPP
#define GODNET_NETWORK_SOCKET_OPS_HPP

#include "godnet/config.hpp"

namespace godnet
{

class InetAddress;

namespace socket_ops
{

GODNET_EXPORT void setNonBlock(int sockfd);

GODNET_EXPORT int createTcpSocket(int family);

GODNET_EXPORT int closeSocket(int sockfd);

GODNET_EXPORT int bindAddress(int sockfd, const InetAddress& addr);

GODNET_EXPORT int listenSocket(int sockfd);

GODNET_EXPORT int acceptSocket(int sockfd, InetAddress& addr);

GODNET_EXPORT int closeWrite(int sockfd);

GODNET_EXPORT int setTcpNoDelay(int sockfd, bool on);

GODNET_EXPORT int setReuseAddr(int sockfd, bool on);

GODNET_EXPORT int setReusePort(int sockfd, bool on);

GODNET_EXPORT int setKeepAlive(int sockfd, bool on);

GODNET_EXPORT int getSocketError(int sockfd);

GODNET_EXPORT int socketPair(int family, int type, int protocol, int fds[2]);

}
}

#endif
