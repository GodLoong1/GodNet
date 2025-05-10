#ifndef GODNET_NETWORK_SOCKET_HPP
#define GODNET_NETWORK_SOCKET_HPP

namespace godnet
{

class Endpoint;

namespace socket
{

void setSocketNonBlock(int sockfd);

int createTcpSocket(int family);

int createUdpSocket(int family);

int closeSocket(int sockfd);

int bindAddress(int sockfd, const Endpoint& endpoint);

int listenSocket(int sockfd);

int acceptSocket(int sockfd, Endpoint& endpoint);

int connectSocket(int sockfd, const Endpoint& endpoint);

int getLocalAddr(int sockfd, Endpoint& addr);

int getPeerAddr(int sockfd, Endpoint& addr);

int closeWrite(int sockfd);

int setTcpNoDelay(int sockfd, bool on);

int setReuseAddr(int sockfd, bool on);

int setReusePort(int sockfd, bool on);

int setKeepAlive(int sockfd, bool on);

int getSocketError(int sockfd);

int createTcpSocketPair(int family, int type, int protocol, int fds[2]);

}
}

#endif
