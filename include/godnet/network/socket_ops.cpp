#include "godnet/network/socket_ops.hpp"

#include <asm-generic/socket.h>
#include <fcntl.h>

#if defined(GODNET_LINUX)
    #include <unistd.h>
    #include <netinet/tcp.h>
    #include <cerrno>
#elif defined(GODNET_WIN)
    #include <ws2tcpip.h>
#endif

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

int bindAddress(int sockfd, struct sockaddr* sockaddr, socklen_t socklen)
{
    return ::bind(sockfd, sockaddr, socklen);
}

int listenSocket(int sockfd)
{
    return ::listen(sockfd, SOMAXCONN);
}

int acceptSocket(int sockfd, struct sockaddr* sockaddr, socklen_t* socklen)
{
#if defined(GODNET_LINUX)
    return ::accept4(sockfd,
                     sockaddr,
                     socklen,
                     SOCK_NONBLOCK | SOCK_CLOEXEC);
#elif defined(GODNET_WIN)
    return ::accept(sockfd,
                    sockaddr,
                    socklen);
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

}