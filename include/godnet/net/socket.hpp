#ifndef GODNET_NET_SOCKET_HPP
#define GODNET_NET_SOCKET_HPP

namespace godnet
{

class InetAddress;

namespace socket
{

void setSocketNonBlock(int sockfd);

int createTcpSocket(int family);

int createUdpSocket(int family);

int closeSocket(int sockfd);

int bindAddress(int sockfd, const InetAddress& localAddr);

int listenSocket(int sockfd);

int acceptSocket(int sockfd, InetAddress& localAddr);

int connectSocket(int sockfd, const InetAddress& localAddr);

int getLocalAddr(int sockfd, InetAddress& localAddr);

int getPeerAddr(int sockfd, InetAddress& peerAddr);

int closeWrite(int sockfd);

int setTcpNoDelay(int sockfd, bool on);

int setReuseAddr(int sockfd, bool on);

int setReusePort(int sockfd, bool on);

int setKeepAlive(int sockfd, bool on);

int getSocketError(int sockfd);

}

}

#endif
