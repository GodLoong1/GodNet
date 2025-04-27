#include "godnet/network/tcp_socket.hpp"

#include <utility>

#include "godnet/util/debug.hpp"
#include "godnet/network/socket.hpp"

namespace godnet
{

TcpSocket TcpSocket::MakeSocket(int family)
{
    int sockfd = socket::createTcpSocket(family);
    if (sockfd < 0)
    {
        GODNET_THROW_RUNERR("socket::createTcpSocket failed");
    }
    return TcpSocket(sockfd);
}

TcpSocket::TcpSocket(int sockfd)
: sockfd_(sockfd)
{
    if (sockfd_ < 0)
    {
        GODNET_THROW_RUNERR("invalid sockfd");
    }
}

TcpSocket::TcpSocket(TcpSocket&& other) noexcept
: TcpSocket()
{
    if (this != &other)
    {
        std::swap(sockfd_, other.sockfd_);
    }
}

TcpSocket& TcpSocket::operator=(TcpSocket&& other) noexcept
{
    if (this != &other)
    {
        std::swap(sockfd_, other.sockfd_);
    }
    return *this;
}

TcpSocket::~TcpSocket()
{
    if (sockfd_ >= 0)
    {
        if (socket::closeSocket(sockfd_) < 0)
        {
            GODNET_ASSERT(false && "socket::closeSocket failed");
        }
    }
}

void TcpSocket::bind(const Endpoint& localaddr)
{
    if (socket::bindAddress(sockfd_, localaddr) < 0)
    {
        GODNET_THROW_SYSERR("socket::bindAddress failed");
    }
}

void TcpSocket::listen()
{
    if (socket::listenSocket(sockfd_) < 0)
    {
        GODNET_THROW_SYSERR("socket::listenSocket failed");
    }
}

TcpSocket TcpSocket::accept(Endpoint& peerEndpoint)
{
    int connfd = socket::acceptSocket(sockfd_, peerEndpoint);
    if (connfd < 0)
    {
        GODNET_THROW_SYSERR("socket::acceptSocket failed");
    }
    return TcpSocket(connfd);
}

void TcpSocket::closeWrite()
{
    if (socket::closeWrite(sockfd_) < 0)
    {
        GODNET_THROW_SYSERR("socket::closeWrite failed");
    }
}

void TcpSocket::setTcpNoDelay(bool on)
{
    if (socket::setTcpNoDelay(sockfd_, on) < 0)
    {
        GODNET_THROW_SYSERR("socket::setTcpNoDelay failed");
    }
}

void TcpSocket::setReuseAddr(bool on)
{
    if (socket::setReuseAddr(sockfd_, on) < 0)
    {
        GODNET_THROW_SYSERR("socket::setReuseAddr failed");
    }
}

void TcpSocket::setReusePort(bool on)
{
    if (socket::setReusePort(sockfd_, on) < 0)
    {
        GODNET_THROW_SYSERR("socket::setReusePort failed");
    }
}

void TcpSocket::setKeepAlive(bool on)
{
    if (socket::setKeepAlive(sockfd_, on) < 0)
    {
        GODNET_THROW_SYSERR("socket::setKeepAlive failed");
    }
}

}