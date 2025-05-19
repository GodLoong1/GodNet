#ifndef GODNET_NET_SOCKET_HPP
#define GODNET_NET_SOCKET_HPP

namespace godnet
{

class InetAddress;

}

namespace godnet::socket
{

void setSocketNonBlock(int sockfd) noexcept;

int createTcpSocket(int family) noexcept;

int createUdpSocket(int family) noexcept;

int closeSocket(int sockfd) noexcept;

int bindAddress(int sockfd, const InetAddress& localAddr) noexcept;

int listenSocket(int sockfd) noexcept;

int acceptSocket(int sockfd, InetAddress& localAddr) noexcept;

int connectSocket(int sockfd, const InetAddress& localAddr) noexcept;

int getLocalAddr(int sockfd, InetAddress& localAddr) noexcept;

int getPeerAddr(int sockfd, InetAddress& peerAddr) noexcept;

int closeWrite(int sockfd) noexcept;

int setTcpNoDelay(int sockfd, bool on) noexcept;

int setReuseAddr(int sockfd, bool on) noexcept;

int setReusePort(int sockfd, bool on) noexcept;

int setKeepAlive(int sockfd, bool on) noexcept;

int getSocketError(int sockfd) noexcept;

}

#endif