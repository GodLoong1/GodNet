#include "godnet/network/tcp_socket.hpp"

#include <cassert>

#include "godnet/network/socket.hpp"
#include "godnet/util/logger.hpp"

namespace godnet
{

TcpSocket::TcpSocket(int sockfd) noexcept
: sockfd_(sockfd)
{
    assert(sockfd_ >= 0);
}

TcpSocket::~TcpSocket()
{
    if (socket::closeSocket(sockfd_) < 0)
    {
        GODNET_LOG_ERROR("socket::closeSocket sockfd: {}", sockfd_);
    }
}

void TcpSocket::bind(const Endpoint& localaddr)
{
    if (socket::bindAddress(sockfd_, localaddr) < 0)
    {
        GODNET_LOG_ERROR("socket::bindAddress sockfd: {}", sockfd_);
    }
}

void TcpSocket::listen()
{
    if (socket::listenSocket(sockfd_) < 0)
    {
        GODNET_LOG_ERROR("socket::listenSocket sockfd: {}", sockfd_);
    }
}

int TcpSocket::accept(Endpoint& peerEndpoint)
{
    return socket::acceptSocket(sockfd_, peerEndpoint);
}

void TcpSocket::closeWrite()
{
    if (socket::closeWrite(sockfd_) < 0)
    {
        GODNET_LOG_ERROR("socket::closeWrite sockfd: {}", sockfd_);
    }
}

void TcpSocket::setTcpNoDelay(bool on)
{
    if (socket::setTcpNoDelay(sockfd_, on) < 0)
    {
        GODNET_LOG_ERROR("socket::setTcpNoDelay sockfd: {}", sockfd_);
    }
}

void TcpSocket::setReuseAddr(bool on)
{
    if (socket::setReuseAddr(sockfd_, on) < 0)
    {
        GODNET_LOG_ERROR("socket::setReuseAddr sockfd: {}", sockfd_);
    }
}

void TcpSocket::setReusePort(bool on)
{
    if (socket::setReusePort(sockfd_, on) < 0)
    {
        GODNET_LOG_ERROR("socket::setReusePort sockfd: {}", sockfd_);
    }
}

void TcpSocket::setKeepAlive(bool on)
{
    if (socket::setKeepAlive(sockfd_, on) < 0)
    {
        GODNET_LOG_ERROR("socket::setKeepAlive sockfd: {}", sockfd_);
    }
}

}