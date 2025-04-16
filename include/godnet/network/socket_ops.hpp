#ifndef GODNET_NETWORK_SOCKET_OPS_HPP
#define GODNET_NETWORK_SOCKET_OPS_HPP

#include "godnet/config.hpp"
#include "godnet/network/socket_types.hpp"

namespace godnet::socket_ops
{

void setNonBlock(int sockfd);

int createTcpSocket(int family);

int closeSocket(int sockfd);

int bindAddress(int sockfd, struct sockaddr* sockaddr, socklen_t socklen);

int listenSocket(int sockfd);

int acceptSocket(int sockfd, struct sockaddr* sockaddr, socklen_t* socklen);

int closeWrite(int sockfd);

int setTcpNoDelay(int sockfd, bool on);

int setReuseAddr(int sockfd, bool on);

int setReusePort(int sockfd, bool on);

int setKeepAlive(int sockfd, bool on);

int getSocketError(int sockfd);

}

#endif
