#include "godnet/net/tcp_socket.hpp"

#ifdef __linux__
    #include <sys/uio.h>
    #include <unistd.h>
#else
    #include <winsock2.h>
#endif

#include <cassert>

#include "godnet/util/logger.hpp"
#include "godnet/util/system.hpp"
#include "godnet/net/socket.hpp"

namespace godnet
{

TcpSocket::TcpSocket(int sockfd)
: sockfd_(sockfd)
{
    assert(sockfd_);
}

TcpSocket::~TcpSocket()
{
    [[maybe_unused]] int ret = forceClose();
    assert(ret);
}

bool TcpSocket::bind(const InetAddress& localAddr)
{
    if (socket::bind(sockfd_, localAddr) < 0)
    {
        GODNET_LOG_ERROR("bind sockfd: {}, localAddr: {}, faild: {}",
            sockfd_,
            localAddr.toIpPort(),
            system::getSystemErrnoMessage());
        return false;
    }
    return true;
}

bool TcpSocket::listen()
{
    if (socket::listen(sockfd_) < 0)
    {
        GODNET_LOG_ERROR("socket::listenSocket sockfd: {}, faild: {}",
            sockfd_,
            system::getSystemErrnoMessage());
        return false;
    }
    return true;
}

int TcpSocket::accept(InetAddress& peerAddr)
{
    return socket::accept(sockfd_, peerAddr);
}

bool TcpSocket::shutdown()
{
    if (socket::shutdown(sockfd_) < 0)
    {
        GODNET_LOG_ERROR("socket::closeWrite sockfd: {}, faild: {}",
            sockfd_,
            system::getSystemErrnoMessage());
        return false;
    }
    return true;
}

bool TcpSocket::forceClose()
{
    if (socket::closeSocket(sockfd_) < 0)
    {
        GODNET_LOG_ERROR("socket::closeSocket sockfd: {}, faild: {}",
            sockfd_,
            system::getSystemErrnoMessage());
        return false;
    }
    return true;
}

std::int64_t TcpSocket::readv(const struct iovec* iov, int count)
{
#ifdef __linux__
    return ::readv(sockfd_, iov, count);
#else
    std::int64_t ret = 0;
    for (int i = 0; i < count; ++i)
    {
        std::int64_t n = ::recv(sockfd_, iov[i].iov_base, iov[i].iov_len, 0);
        if (n == iov[i].iov_len)
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

std::int64_t TcpSocket::write(const char* buf, std::size_t len)
{
#ifdef __linux__
    return ::write(sockfd_, buf, len);
#else
    return ::send(sockfd_, buf, len, 0);
#endif
}

InetAddress TcpSocket::getLocalAddr()
{
    InetAddress localAddr;
    socklen_t socklen = localAddr.getSockLen();
    if (::getsockname(sockfd_, localAddr.mutSockAddr(), &socklen) < 0)
    {
        GODNET_LOG_ERROR("getsockname() sockfd: {}, faild: {}",
            sockfd_,
            system::getSystemErrnoMessage());
    }
    return localAddr;
}

InetAddress TcpSocket::getPeerAddr()
{
    InetAddress peerAddr;
    socklen_t socklen = peerAddr.getSockLen();
    if (::getpeername(sockfd_, (struct sockaddr*)&peerAddr, &socklen) < 0)
    {
        GODNET_LOG_ERROR("getpeername() sockfd: {}, faild: {}",
            sockfd_,
            system::getSystemErrnoMessage());
    }
    return peerAddr;
}

bool TcpSocket::setTcpNoDelay(bool on)
{
    if (socket::setTcpNoDelay(sockfd_, on) < 0)
    {
        GODNET_LOG_ERROR("socket::setTcpNoDelay sockfd: {}, faild: {}",
            sockfd_,
            system::getSystemErrnoMessage());
        return false;
    }
    return true;
}

bool TcpSocket::setReuseAddr(bool on)
{
    if (socket::setReuseAddr(sockfd_, on) < 0)
    {
        GODNET_LOG_ERROR("socket::setReuseAddr sockfd: {}, faild: {}",
            sockfd_,
            system::getSystemErrnoMessage());
        return false;
    }
    return true;
}

bool TcpSocket::setReusePort(bool on)
{
    if (socket::setReusePort(sockfd_, on) < 0)
    {
        GODNET_LOG_ERROR("socket::setReusePort sockfd: {}, faild: {}",
            sockfd_,
            system::getSystemErrnoMessage());
        return false;
    }
    return true;
}

bool TcpSocket::setKeepAlive(bool on)
{
    if (socket::setKeepAlive(sockfd_, on) < 0)
    {
        GODNET_LOG_ERROR("socket::setKeepAlive sockfd: {}, faild: {}",
            sockfd_,
            system::getSystemErrnoMessage());
        return false;
    }
    return true;
}

}