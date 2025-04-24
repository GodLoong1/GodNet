#include "godnet/network/tcp_acceptor.hpp"

#include "godnet/network/tcp_socket.hpp"
#include "godnet/network/event_loop.hpp"
#include "godnet/network/event_channel.hpp"
#include "godnet/util/debug.hpp"

namespace godnet
{

TcpAcceptor::TcpAcceptor(EventLoop* loop,
                         const InetAddress& addr,
                         bool reuseAddr,
                         bool reusePort)
: loop_(loop),
  addr_(addr),
  socket_(TcpSocket::CreateSocket(addr.family())),
  channel_(std::make_unique<EventChannel>(loop_, socket_.fd()))
{
    socket_.setReuseAddr(reuseAddr);
    socket_.setReusePort(reusePort);
    socket_.bind(addr_);
    channel_->setReadCallback([this]() { handleRead(); });
}

void TcpAcceptor::listen()
{
    loop_->assertInLoop();
    socket_.listen();
    channel_->enableReading();
}

void TcpAcceptor::handleRead()
{
    InetAddress peerAddr;
    int newSockfd = socket_.accept(peerAddr);
    if (newSockfd >= 0)
    {
        if (newConnectionCallback_)
        {
            newConnectionCallback_(newSockfd, peerAddr);
        }
        else
        {
        }
    }
    else
    {
    }
}

}