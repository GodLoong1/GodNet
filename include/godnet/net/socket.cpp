#include "godnet/net/socket.hpp"
#include "godnet/net/inet_address.hpp"
#include "godnet/util/system.hpp"

#ifdef __linux__
    #include <unistd.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>
    #include <fcntl.h>
    #include <sys/uio.h>
#else
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif

namespace godnet::socket
{

void setNonBlock(int sockfd) noexcept
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

int tcpSocket(int family) noexcept
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
        setNonBlack(sockfd);
    }
    return sockfd;
#endif
}

int udpSocket(int family) noexcept
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
        setNonBlack(sockfd);
    }
    return sockfd;
#endif
}

int closeSocket(int sockfd) noexcept
{
#ifdef __linux__
        return ::close(sockfd);
#else
        return ::closesocket(sockfd);
#endif
}

int bind(int sockfd, const InetAddress& localAddr) noexcept
{
    return ::bind(sockfd, localAddr.getSockAddr(), localAddr.getSockLen());
}

int listen(int sockfd) noexcept
{
    return ::listen(sockfd, SOMAXCONN);
}

int accept(int sockfd, InetAddress& peerAddr) noexcept
{
    socklen_t socklen = peerAddr.getSockLen();
#ifdef __linux__
    return ::accept4(sockfd,
                     peerAddr.mutSockAddr(),
                     &socklen,
                     SOCK_NONBLOCK | SOCK_CLOEXEC);
#else
    return ::accept(sockfd,
                    peerAddr.mutSockAddr(),
                    &socklen);
#endif
}


int connect(int sockfd, const InetAddress& localAddr) noexcept
{
    int ret = ::connect(sockfd,
                        localAddr.getSockAddr(),
                        localAddr.getSockLen());
    int err = ret ? system::getErrno() : ret;
    switch (err)
    {
        case 0:
        case EINPROGRESS:
        case EINTR:
        case EISCONN:
        {
            ret = 0;
            break;
        }
        case EAGAIN:
        case ECONNREFUSED:
        case ENETUNREACH:
        default:
        {
            ret = -1;
            break;
        }
    }
    return ret;
}

std::int64_t read(int sockfd, char* buf[2], std::size_t len[2],
                  std::size_t count)
{
#ifdef __linux__
    struct iovec iov[2];
    iov[0].iov_base = buf[0];
    iov[0].iov_len = len[0];
    iov[1].iov_base = buf[1];
    iov[1].iov_len = len[1];
    return ::readv(sockfd, iov, count);
#else
    std::int64_t ret = 0;
    for (std::size_t i = 0; i != count; ++i)
    {
        std::int64_t n = ::recv(sockfd_, buf[i], len[i], 0);
        if (n == len[i])
        {
            ret += n;
        }
        else
        {
            if (n < 0)
            {
                ret = (ret == 0 ? n : ret);
            }
            else
            {
                ret += n;
            }
            break;
        }
    }
    return ret;
#endif
}

std::int64_t write(int sockfd, const char* buf, std::size_t len)
{
#ifdef __linux__
    return ::write(sockfd, buf, len);
#else
    return ::send(sockfd, buf, len, 0);
#endif
}

int getLocalAddr(int sockfd, InetAddress& localAddr) noexcept
{
    socklen_t socklen = localAddr.getSockLen();
    return ::getsockname(sockfd,
                         localAddr.mutSockAddr(),
                         &socklen);
}

int getPeerAddr(int sockfd, InetAddress& peerAddr) noexcept
{
    socklen_t socklen = peerAddr.getSockLen();
    return ::getpeername(sockfd,
                         peerAddr.mutSockAddr(),
                         &socklen);
}

int shutdown(int sockfd) noexcept
{
#ifdef __linux__
    return ::shutdown(sockfd, SHUT_WR);
#else
    return ::shutdown(sockfd, SD_SEND);
#endif
}

int setTcpNoDelay(int sockfd, bool on) noexcept
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

int setReuseAddr(int sockfd, bool on) noexcept
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

int setReusePort(int sockfd, bool on) noexcept
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

int setKeepAlive(int sockfd, bool on) noexcept
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

int getSocketError(int sockfd) noexcept
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

bool isSelfConnect(int sockfd) noexcept
{
    InetAddress localAddr;
    if (getLocalAddr(sockfd, localAddr) < 0)
    {
        return false;
    }
    InetAddress peerAddr;
    if (getPeerAddr(sockfd, peerAddr) < 0)
    {
        return false;
    }
    return localAddr == peerAddr;
}

}