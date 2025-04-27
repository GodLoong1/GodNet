#include "godnet/network/tcp_acceptor.hpp"

#include "godnet/network/endpoint.hpp"
#include "godnet/network/tcp_socket.hpp"
#include "godnet/network/event_loop.hpp"
#include "godnet/network/event_channel.hpp"

namespace godnet
{

TcpAcceptor::TcpAcceptor(EventLoop* loop,
                         const Endpoint& endpoint,
                         bool reuseAddr,
                         bool reusePort)
: loop_(loop),
  endpoint_(endpoint),
  socket_(TcpSocket::MakeSocket(endpoint.family())),
  channel_(std::make_unique<EventChannel>(loop_, socket_.getSockfd()))
{
    socket_.setReuseAddr(reuseAddr);
    socket_.setReusePort(reusePort);
    socket_.bind(endpoint_);
    channel_->setReadCallback([this]() { handleRead(); });
}

TcpAcceptor::~TcpAcceptor()
{
    loop_->assertInLoop();
    channel_->disableAll();
}

void TcpAcceptor::listen()
{
    loop_->assertInLoop();
    socket_.listen();
    channel_->enableReading();
}

void TcpAcceptor::handleRead()
{
    auto peerAddr{makeEndpointV4()};
    TcpSocket connSocket = socket_.accept(peerAddr);
    if (newConnectionCallback_)
    {
        newConnectionCallback_(std::move(connSocket), peerAddr);
    }
}

}