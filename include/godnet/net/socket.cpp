#include "godnet/net/socket.hpp"

#ifdef __linux__
    #include <unistd.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>
    #include <fcntl.h>
#else
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif

#include "godnet/net/inet_address.hpp"

namespace godnet::socket
{

void setSocketNonBlock(int sockfd)
{
#ifdef __linux__
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    ::fcntl(sockfd, F_SETFL, flags);

    flags = ::fcntl(sockfd, F_GETFD, 0);
    flags |= FD_CLOEXEC;
    ::fcntl(sockfd, F_SETFD, flags);
#else
    u_long arg = 1;
    ::ioctlsocket(sockfd, FIONBIO, &arg);
#endif
}

int createTcpSocket(int family)
{
#ifdef __linux__
    return ::socket(family,
                    SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
                    IPPROTO_TCP);
#else
    int sockfd = static_cast<int>(::socket(family,
                                           SOCK_STREAM,
                                           IPPROTO_TCP));
    if (sockfd >= 0)
    {
        setSocketNonBlock(sockfd);
    }
    return sockfd;
#endif
}

int createUdpSocket(int family)
{
#ifdef __linux__
    return ::socket(family,
                    SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
                    IPPROTO_UDP);
#else
    int sockfd = static_cast<int>(::socket(family,
                                           SOCK_DGRAM,
                                           IPPROTO_UDP));
    if (sockfd >= 0)
    {
        setSocketNonBlock(sockfd);
    }
    return sockfd;
#endif
}

int closeSocket(int sockfd)
{
#ifdef __linux__
        return ::close(sockfd);
#else
        return ::closesocket(sockfd);
#endif
}

int bindAddress(int sockfd, const InetAddress& endpoint)
{
    return ::bind(sockfd, endpoint.getSockAddr(), endpoint.getSockLen());
}

int listenSocket(int sockfd)
{
    return ::listen(sockfd, SOMAXCONN);
}

int acceptSocket(int sockfd, InetAddress& endpoint)
{
    socklen_t socklen = endpoint.getSockLen();
#ifdef __linux__
    return ::accept4(sockfd,
                     endpoint.mutSockAddr(),
                     &socklen,
                     SOCK_NONBLOCK | SOCK_CLOEXEC);
#else
    return ::accept(sockfd,
                    endpoint.mutSockAddr(),
                    &socklen);
#endif
}


int connectSocket(int sockfd, const InetAddress& endpoint)
{
#ifdef __linux__
    return ::connect(sockfd,
                     endpoint.getSockAddr(),
                     endpoint.getSockLen());
#else
    return ::connect(sockfd,
                     endpoint.getSockAddr(),
                     endpoint.getSockLen());
#endif
}

int getLocalAddr(int sockfd, InetAddress& endpoint)
{
    socklen_t socklen = endpoint.getSockLen();
    return ::getsockname(sockfd,
                         endpoint.mutSockAddr(),
                         &socklen);
}

int getPeerAddr(int sockfd, InetAddress& endpoint)
{
    socklen_t socklen = endpoint.getSockLen();
    return ::getpeername(sockfd,
                         endpoint.mutSockAddr(),
                         &socklen);
}

int closeWrite(int sockfd)
{
#ifdef __linux__
    return ::shutdown(sockfd, SHUT_WR);
#else
    return ::shutdown(sockfd, SD_SEND);
#endif
}

int setTcpNoDelay(int sockfd, bool on)
{
#ifdef __linux__
    int optval = on ? 1 : 0;
#else
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
#ifdef __linux__
    int optval = on ? 1 : 0;
#else
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
#ifdef __linux__
    int optval = on ? 1 : 0;
    return ::setsockopt(sockfd,
                        SOL_SOCKET,
                        SO_REUSEPORT,
                        &optval,
                        sizeof(optval));
#else
    return 0;
#endif
}

int setKeepAlive(int sockfd, bool on)
{
#ifdef __linux__
    int optval = on ? 1 : 0;
#else
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
#ifdef __linux__
    int optval;
#else
    char optval;
#endif
    socklen_t optlen = static_cast<socklen_t>(sizeof optval);

    if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
    {
#ifdef __linux__
        return errno;
#else
        return ::WSAGetLastError();
#endif
    }
    return optval;
}

}