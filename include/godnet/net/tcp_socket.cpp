#include "godnet/net/tcp_socket.hpp"

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
    if (socket::bindAddress(sockfd_, localAddr) < 0)
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
    if (socket::listenSocket(sockfd_) < 0)
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
    return socket::acceptSocket(sockfd_, peerAddr);
}

bool TcpSocket::shutdown()
{
    if (socket::closeWrite(sockfd_) < 0)
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