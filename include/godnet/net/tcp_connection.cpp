#include "godnet/net/tcp_connection.hpp"

#include <cassert>

#include "godnet/net/event_loop.hpp"
#include "godnet/net/event_channel.hpp"

namespace godnet
{

TcpConnection::TcpConnection(EventLoop* loop,
                             int sockfd,
                             const InetAddress& peerAddr)
: loop_(loop),
  socket_(sockfd),
  channel_(std::make_unique<EventChannel>(loop, sockfd)),
  localAddr_(socket_.getLocalAddr()),
  peerAddr_(peerAddr)
{
    assert(loop);
    assert(localAddr_.isValid());
    assert(peerAddr_.isValid());
}

TcpConnection::~TcpConnection()
{
    assert(status_ == Status::disconnected);
}

void TcpConnection::init()
{
    if (loop_->isInLoopThread())
    {
        initInLoop();
    }
    else
    {
        loop_->queueInLoop([self(shared_from_this())] {
            self->initInLoop();
        });
    }
}

void TcpConnection::initInLoop()
{
    loop_->assertInLoopThread();

    assert(status_ == Status::disconnected);
    status_ = Status::connected;
    channel_->bindWeakPtr(weak_from_this());
    channel_->enableReading();

    connectionCallback_(shared_from_this());
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
    if (loop_->isInLoopThread())
    {
        sendInLoop(buf, len);
    }
    else
    {
        loop_->queueInLoop(
            [self(shared_from_this()), buffer(std::string(buf, len))] {
                self->sendInLoop(buffer.c_str(), buffer.size());
        });
    }
}

void TcpConnection::send(const std::string& buf)
{
    if (loop_->isInLoopThread())
    {
        sendInLoop(buf.c_str(), buf.size());
    }
    else
    {
        loop_->queueInLoop(
            [self(shared_from_this()), buffer(buf)] {
                self->sendInLoop(buffer.c_str(), buffer.size());
        });
    }
}

void TcpConnection::send(std::string&& buf)
{
    if (loop_->isInLoopThread())
    {
        sendInLoop(buf.c_str(), buf.size());
    }
    else
    {
        loop_->queueInLoop(
            [self(shared_from_this()), buffer(std::move(buf))] {
                self->sendInLoop(buffer.c_str(), buffer.size());
        });
    }
}

void TcpConnection::send(const MessageBuffer& buf)
{
    if (loop_->isInLoopThread())
    {
        sendInLoop(buf.data(), buf.size());
    }
    else
    {
        loop_->queueInLoop(
            [self(shared_from_this()),
             buffer(std::string(buf.data(), buf.size()))] {
                self->sendInLoop(buffer.c_str(), buffer.size());
        });
    }
}

void TcpConnection::sendInLoop(const char* buf, std::size_t len)
{
    loop_->assertInLoopThread();

    std::int64_t n = 0;
    if (!channel_->isWriting() && writeBuffer_.empty())
    {
        n = socket_.write(buf, len);
        if (n < 0)
        {
            n = 0;
        }
    }
    if (static_cast<std::size_t>(n) < len)
    {
        writeBuffer_.write(buf + n, len - n);
        if (!channel_->isWriting())
        {
            channel_->enableWriting();
        }
    }
}

void TcpConnection::handleRead()
{
    loop_->assertInLoopThread();

    char extBuffer[8192];
    char* buf[2];
    std::size_t len[2];
    buf[0] = readBuffer_.mutWritePeek();
    len[0] = readBuffer_.writeByte();
    buf[1] = extBuffer;
    len[1] = sizeof(extBuffer);
    int count = (len[0] < sizeof(extBuffer)) ? 2 : 1;

    std::int64_t n = socket_.read(buf, len, count);
    if (n > 0)
    {
        if (static_cast<std::size_t>(n) <= len[0])
        {
            readBuffer_.advanceWrite(n);
        }
        else
        {
            readBuffer_.advanceWrite(len[0]);
            readBuffer_.write(extBuffer, n - len[0]);
        }

        messageCallback_(shared_from_this(), readBuffer_);
    }
    else if (n == 0)
    {
        handleClose();
    }
    else
    {
        handleClose();
    }
}

void TcpConnection::handleWrite()
{
    loop_->assertInLoopThread();

    if (channel_->isWriting())
    {
        std::int64_t n = socket_.write(writeBuffer_.readPeek(),
                                       writeBuffer_.readByte());
        if (n > 0)
        {
            writeBuffer_.retrieve(n);
            if (writeBuffer_.empty())
            {
                channel_->disableWriting();
                if (status_ == Status::disconnecting)
                {
                    socket_.shutdown();
                }
            }
        }
        else
        {
            // TODO log
        }
    }
}

void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();

    assert(status_ == Status::connected || status_ == Status::disconnecting);
    status_ = Status::disconnected;
    channel_->disableAll();

    auto self = shared_from_this();
    connectionCallback_(self);
    closeCallback_(self);
}

void TcpConnection::handleError()
{

}

}