#ifndef GODNET_NETWORK_SOCKET_HPP
#define GODNET_NETWORK_SOCKET_HPP

#include "godnet/config.hpp"

namespace godnet
{

class Endpoint;

namespace socket
{

GODNET_EXPORT void setSocketNonBlock(int sockfd);

GODNET_EXPORT int createTcpSocket(int family);

GODNET_EXPORT int createUdpSocket(int family);

GODNET_EXPORT int closeSocket(int sockfd);

GODNET_EXPORT int bindAddress(int sockfd, const Endpoint& endpoint);

GODNET_EXPORT int listenSocket(int sockfd);

GODNET_EXPORT int acceptSocket(int sockfd, Endpoint& endpoint);

GODNET_EXPORT int connectSocket(int sockfd, const Endpoint& endpoint);

GODNET_EXPORT int getLocalAddr(int sockfd, Endpoint& addr);

GODNET_EXPORT int getPeerAddr(int sockfd, Endpoint& addr);

GODNET_EXPORT int closeWrite(int sockfd);

GODNET_EXPORT int setTcpNoDelay(int sockfd, bool on);

GODNET_EXPORT int setReuseAddr(int sockfd, bool on);

GODNET_EXPORT int setReusePort(int sockfd, bool on);

GODNET_EXPORT int setKeepAlive(int sockfd, bool on);

GODNET_EXPORT int getSocketError(int sockfd);

GODNET_EXPORT int createTcpSocketPair(int family, int type, int protocol, int fds[2]);

}
}

#endif
