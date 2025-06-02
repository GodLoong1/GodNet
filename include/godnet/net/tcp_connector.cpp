#include "godnet/net/tcp_connector.hpp"

#include <cassert>

#include "godnet/net/socket.hpp"
#include "godnet/net/event_channel.hpp"
#include "godnet/net/event_loop.hpp"

namespace godnet
{

TcpConnector::TcpConnector(EventLoop* loop,
                           const InetAddress& peerAddr,
                           TimerDuration retryDelay,
                           TimerDuration maxDelay) noexcept
: loop_(loop),
  peerAddr_(peerAddr),
  retryDelay_(retryDelay),
  currDelay_(retryDelay),
  maxDelay_(maxDelay)
{
    assert(loop);
    assert(peerAddr.isValid());
    assert(maxDelay >= retryDelay);
}

TcpConnector::~TcpConnector() noexcept
{
    loop_->assertInLoopThread();

    if (timerId_)
    {
        loop_->cancelTimer(timerId_);
        timerId_ = 0;
    }
    if (status_ == Status::connecting)
    {
        int sockfd = resetChannel();
        socket::closeSocket(sockfd);
        status_ = Status::disconnected;
    }
    assert(!channel_);
}

void TcpConnector::connect() noexcept
{
    loop_->assertInLoopThread();
    assert(status_ == Status::disconnected);
    assert(!channel_);
    assert(newConnectionCallback_);

    if (timerId_)
    {
        timerId_ = 0;
    }

    int sockfd = socket::tcpSocket(peerAddr_.family());
    if (!socket::connect(sockfd, peerAddr_))
    {
        status_ = Status::connecting;
        channel_ = std::make_shared<EventChannel>(loop_, sockfd);
        channel_->setWriteCallback([this] {
            handleWrite();
        });
        // 防止 restChannel 中析构
        channel_->bindWeakPtr(channel_);
        channel_->enableWriting();
    }
    else
    {
        retry(sockfd);
    }
}

void TcpConnector::reconnect() noexcept
{
    loop_->assertInLoopThread();
    assert(status_ == Status::connected);
    assert(!channel_);

    status_ = Status::disconnected;
    currDelay_ = retryDelay_;
    connect();
}

void TcpConnector::retry(int sockfd) noexcept
{
    loop_->assertInLoopThread();
    assert(status_ == Status::disconnected);
    assert(!channel_);
    assert(!timerId_);

    socket::closeSocket(sockfd);
    timerId_ = loop_->runTimerAfter(currDelay_, [this] {
        connect();
    });

    currDelay_ *= 2;
    if (currDelay_ > maxDelay_)
    {
        currDelay_ = maxDelay_;
    }
}

int TcpConnector::resetChannel() noexcept
{
    loop_->assertInLoopThread();
    assert(status_ == Status::connecting);
    assert(channel_);

    channel_->disableAll();
    int sockfd = channel_->fd();
    channel_.reset();

    return sockfd;
}

void TcpConnector::handleWrite() noexcept
{
    loop_->assertInLoopThread();
    assert(status_ == Status::connecting);

    int sockfd = resetChannel();
    int err = socket::getSocketError(sockfd);

    if (err || socket::isSelfConnect(sockfd))
    {
        status_ = Status::disconnected;
        retry(sockfd);
    }
    else
    {
        status_ = Status::connected;
        newConnectionCallback_(sockfd);
    }
}

}