#include "godnet/net/tcp_connection.hpp"

#include <cassert>

#include "godnet/net/event_loop.hpp"
#include "godnet/net/event_channel.hpp"

namespace godnet
{

TcpConnection::TcpConnection(EventLoop* loop,
                             int sockfd,
                             const InetAddress& localAddr,
                             const InetAddress& peerAddr)
: loop_(loop),
  socket_(sockfd),
  channel_(std::make_unique<EventChannel>(loop, sockfd)),
  localAddr_(localAddr),
  peerAddr_(peerAddr)
{
    assert(loop);
    assert(localAddr_.isValid());
    assert(peerAddr_.isValid());
}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::shutdown()
{
    if (loop_->isInLoopThread())
    {
        shutdownInLoop();
    }
    else
    {
        loop_->queueInLoop([self(shared_from_this())]() {
            self->shutdownInLoop();
        });
    }
}

void TcpConnection::shutdownInLoop()
{
    if (status_ == Status::connected)
    {
        status_ = Status::disconnecting;
        if (!channel_->isWriting())
        {
            socket_.shutdown();
        }
    }
}

void TcpConnection::forceClose()
{
    if (loop_->isInLoopThread())
    {
        forceCloseInLoop();
    }
    else
    {
        loop_->queueInLoop([self(shared_from_this())]() {
            self->forceCloseInLoop();
        });
    }
}

void TcpConnection::forceCloseInLoop()
{
    if (status_ == Status::connected || status_ == Status::disconnecting)
    {
        status_ = Status::disconnected;
        handleClose();
    }
}

void TcpConnection::send(const char* buf, std::size_t len)
{

}

void TcpConnection::send(const std::string& buf)
{

}

void TcpConnection::send(std::string&& buf)
{

}

void TcpConnection::send(const MessageBuffer& buf)
{

}

void TcpConnection::handleRead()
{

}

void TcpConnection::handleWrite()
{

}

void TcpConnection::handleClose()
{

}

void TcpConnection::handleError()
{

}

}