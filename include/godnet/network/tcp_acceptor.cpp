#include "godnet/network/tcp_acceptor.hpp"

#include "godnet/network/socket.hpp"
#include "godnet/network/endpoint.hpp"
#include "godnet/network/tcp_socket.hpp"
#include "godnet/network/event_loop.hpp"
#include "godnet/network/event_channel.hpp"
#include "godnet/util/logger.hpp"

namespace godnet
{

TcpAcceptor::TcpAcceptor(EventLoop* loop,
                         const Endpoint& endpoint,
                         bool reuseAddr,
                         bool reusePort)
: loop_(loop),
  endpoint_(endpoint),
  socket_(socket::createTcpSocket(endpoint.family())),
  channel_(std::make_unique<EventChannel>(loop_, socket_.fd()))
{
    socket_.setReuseAddr(reuseAddr);
    socket_.setReusePort(reusePort);
    socket_.bind(endpoint_);
    channel_->setReadCallback([this]() { handleRead(); });
}

TcpAcceptor::~TcpAcceptor()
{
    loop_->assertInLoopThread();

    channel_->disableAll();
}

void TcpAcceptor::listen()
{
    loop_->assertInLoopThread();

    socket_.listen();
    channel_->enableReading();
}

void TcpAcceptor::handleRead()
{
    Endpoint peerAddr;
    int connfd = socket_.accept(peerAddr);
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
        GODNET_LOG_ERROR("socket_.accept fd: {}", socket_.fd());
    }
}

}