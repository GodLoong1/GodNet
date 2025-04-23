#include "godnet/network/socket_ops.hpp"

#include <fcntl.h>
#include <sys/socket.h>

#if defined(GODNET_LINUX)
    #include <unistd.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>
#elif defined(GODNET_WIN)
    #include <ws2tcpip.h>
#endif

#include "godnet/network/inet_address.hpp"

namespace godnet::socket_ops
{

void setNonBlock(int sockfd)
{
#if defined(GODNET_LINUX)
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    ::fcntl(sockfd, F_SETFL, flags);

    flags = ::fcntl(sockfd, F_GETFD, 0);
    flags |= FD_CLOEXEC;
    ::fcntl(sockfd, F_SETFD, flags);
#elif defined(GODNET_WIN)
    u_long arg = 1;
    ::ioctlsocket(sockfd, FIONBIO, &arg);
#endif
}

int createTcpSocket(int family)
{
#if defined(GODNET_LINUX)
    return ::socket(family,
                    SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
                    IPPROTO_TCP);
#elif defined(GODNET_WIN)
    return static_cast<int>(::socket(family,
                                     SOCK_STREAM,
                                     IPPROTO_TCP));
#endif
}

int closeSocket(int sockfd)
{
#if defined(GODNET_LINUX)
        return ::close(sockfd);
#elif defined(GODNET_WIN)
        return closesocket(sockfd);
#endif
}

int bindAddress(int sockfd, const InetAddress& addr)
{
    return ::bind(sockfd, addr.getSockAddr(), addr.getSockLen());
}

int listenSocket(int sockfd)
{
    return ::listen(sockfd, SOMAXCONN);
}

int acceptSocket(int sockfd, InetAddress& addr)
{
    socklen_t socklen = addr.getSockLen();
#if defined(GODNET_LINUX)
    return ::accept4(sockfd,
                     addr.mutSockAddr(),
                     &socklen,
                     SOCK_NONBLOCK | SOCK_CLOEXEC);
#elif defined(GODNET_WIN)
    return ::accept(sockfd,
                    addr.mutSockAddr(),
                    &socklen);
#endif
}


int connectSocket(int sockfd, const InetAddress& addr)
{
#if defined(GODNET_LINUX)
    return ::connect(sockfd,
                     addr.getSockAddr(),
                     addr.getSockLen());
#elif defined(GODNET_WIN)
    return ::connect(sockfd,
                     addr.getSockAddr(),
                     addr.getSockLen());
#endif
}

int getSocketName(int sockfd, InetAddress& addr)
{
    socklen_t socklen = addr.getSockLen();
#if defined(GODNET_LINUX)
    return ::getsockname(sockfd,
                         addr.mutSockAddr(),
                         &socklen);
#elif defined(GODNET_WIN)
    return ::getsockname(sockfd,
                         addr.mutSockAddr(),
                         &socklen);
#endif
}

int closeWrite(int sockfd)
{
#if defined(GODNET_LINUX)
    return ::shutdown(sockfd, SHUT_WR);
#elif defined(GODNET_WIN)
    return ::shutdown(sockfd, SD_SEND);
#endif
}

int setTcpNoDelay(int sockfd, bool on)
{
#if defined(GODNET_LINUX)
    int optval = on ? 1 : 0;
#elif defined(GODNET_WIN)
    char optval = on ? 1 : 0;
#endif
    return ::setsockopt(sockfd,
                        IPPROTO_TCP,
                        TCP_NODELAY,
                        &optval,
                        sizeof(optval));
}

int setReuseAddr(int sockfd, bool on)
{
#if defined(GODNET_LINUX)
    int optval = on ? 1 : 0;
#elif defined(GODNET_WIN)
    char optval = on ? 1 : 0;
#endif
    return ::setsockopt(sockfd,
                        SOL_SOCKET,
                        SO_REUSEADDR,
                        &optval,
                        sizeof(optval));
}

int setReusePort(int sockfd, bool on)
{
#if defined(GODNET_LINUX)
    int optval = on ? 1 : 0;
#elif defined(GODNET_WIN)
    char optval = on ? 1 : 0;
#endif
    return ::setsockopt(sockfd,
                        SOL_SOCKET,
                        SO_REUSEPORT,
                        &optval,
                        sizeof(optval));
}

int setKeepAlive(int sockfd, bool on)
{
#if defined(GODNET_LINUX)
    int optval = on ? 1 : 0;
#elif defined(GODNET_WIN)
    char optval = on ? 1 : 0;
#endif
    return ::setsockopt(sockfd,
                        SOL_SOCKET,
                        SO_KEEPALIVE,
                        &optval,
                        sizeof(optval));
}

int getSocketError(int sockfd)
{
#if defined(GODNET_LINUX)
    int optval;
#elif defined(GODNET_WIN)
    char optval;
#endif
    socklen_t optlen = static_cast<socklen_t>(sizeof optval);

    if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
    {
#if defined(GODNET_LINUX)
        return errno;
#elif defined(GODNET_WIN)
        return ::WSAGetLastError();
#endif
    }
    return optval;
}

int createTcpSocketPair(int family, int fds[2])
{
    if (family != AF_INET || family != AF_INET6)
    {
        return -1;
    }
#if defined(GODNET_LINUX)
    return ::socketpair(family,
                        SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
                        0,
                        fds);
#elif defined(GODNET_WIN)
    int listenfd = -1, connfd = -1, acceptfd = -1;
    InetAddress localaddr(0, true, family == AF_INET6);

    listenfd = createTcpSocket(family);
    if (listenfd < 0)
    {
        goto error;
    }
    if (bindAddress(listenfd, localaddr) < 0)
    {
        goto error;
    }
    if (listenSocket(listenfd) < 0)
    {
        goto error;
    }
    if (getSocketName(listenfd, localaddr) < 0)
    {
        goto error;
    }
    connfd = createTcpSocket(family);
    if (connfd < 0)
    {
        goto error;
    }
    if (connectSocket(connfd, localaddr))
    {
        goto error;
    }
    acceptfd = acceptSocket(listenfd, localaddr);
    if (acceptfd < 0)
    {
        goto error;
    }
    closeSocket(listenfd);
    fds[0] = connfd;
    fds[1] = acceptfd;
    return 0;
error:
    if (listenfd >= 0)
    {
        closeSocket(listenfd);
    }
    if (connfd >= 0)
    {
        closeSocket(connfd);
    }
    if (acceptfd >= 0)
    {
        closeSocket(acceptfd);
    }
    return -1;
#endif
}

}