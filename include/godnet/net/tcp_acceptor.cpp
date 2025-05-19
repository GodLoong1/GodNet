#include "godnet/net/tcp_acceptor.hpp"

#include "godnet/util/logger.hpp"
#include "godnet/util/system.hpp"
#include "godnet/net/socket.hpp"
#include "godnet/net/inet_address.hpp"
#include "godnet/net/tcp_socket.hpp"
#include "godnet/net/event_loop.hpp"
#include "godnet/net/event_channel.hpp"

namespace godnet
{

TcpAcceptor::TcpAcceptor(EventLoop* loop, const InetAddress& localAddr) noexcept
: loop_(loop),
  localAddr_(localAddr)
{
    assert(loop);
    assert(localAddr.valid());

    int sockfd = socket::createTcpSocket(localAddr.family());
    assert(sockfd >= 0);

    socket_ = std::make_unique<TcpSocket>(sockfd);
    socket_->bind(localAddr_);

    channel_ = std::make_unique<EventChannel>(loop, socket_->fd());
    channel_->setReadCallback([this]() { handleRead(); });
}

TcpAcceptor::~TcpAcceptor()
{
    loop_->assertInLoopThread();

    channel_->disableAll();
}

bool TcpAcceptor::listen() noexcept
{
    loop_->assertInLoopThread();

    if (!socket_->listen())
    {
        return false;
    }
    channel_->enableReading();
    return true;
}

bool TcpAcceptor::setReuseAddr(bool on) noexcept
{
    loop_->assertInLoopThread();

    return socket_->setReuseAddr(on);
}

bool TcpAcceptor::setReusePort(bool on) noexcept
{
    loop_->assertInLoopThread();

    return socket_->setReusePort(on);
}

void TcpAcceptor::handleRead()
{
    InetAddress peerAddr;
    int connfd = socket_->accept(peerAddr);
    if (connfd >= 0)
    {
        if (newConnectionCallback_)
        {
            newConnectionCallback_(connfd, peerAddr);
        }
        else
        {
            socket::closeSocket(connfd);
        }
    }
    else
    {
        GODNET_LOG_ERROR("accept fd: {}, fail: {}",
            socket_->fd(),
            system::getSystemErrnoMessage());
    }
}

}