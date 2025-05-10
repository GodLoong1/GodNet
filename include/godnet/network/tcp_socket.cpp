#include "godnet/network/tcp_socket.hpp"

#include <utility>
#include <cassert>

#include "godnet/network/socket.hpp"

namespace godnet
{

TcpSocket TcpSocket::MakeSocket(int family)
{
    int sockfd = socket::createTcpSocket(family);
    assert(sockfd >= 0);
    return TcpSocket(sockfd);
}

TcpSocket::TcpSocket(int sockfd)
: sockfd_(sockfd)
{
    assert(sockfd_ >= 0);
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
        }
    }
}

void TcpSocket::bind(const Endpoint& localaddr)
{
    if (socket::bindAddress(sockfd_, localaddr) < 0)
    {
    }
}

void TcpSocket::listen()
{
    if (socket::listenSocket(sockfd_) < 0)
    {
    }
}

TcpSocket TcpSocket::accept(Endpoint& peerEndpoint)
{
    int connfd = socket::acceptSocket(sockfd_, peerEndpoint);
    if (connfd < 0)
    {
    }
    return TcpSocket(connfd);
}

void TcpSocket::closeWrite()
{
    if (socket::closeWrite(sockfd_) < 0)
    {
    }
}

void TcpSocket::setTcpNoDelay(bool on)
{
    if (socket::setTcpNoDelay(sockfd_, on) < 0)
    {
    }
}

void TcpSocket::setReuseAddr(bool on)
{
    if (socket::setReuseAddr(sockfd_, on) < 0)
    {
    }
}

void TcpSocket::setReusePort(bool on)
{
    if (socket::setReusePort(sockfd_, on) < 0)
    {
    }
}

void TcpSocket::setKeepAlive(bool on)
{
    if (socket::setKeepAlive(sockfd_, on) < 0)
    {
    }
}

}