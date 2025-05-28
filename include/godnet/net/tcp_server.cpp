#include "godnet/net/tcp_server.hpp"

#include <cassert>

namespace godnet
{

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr)
: loop_(loop),
  acceptor_(std::make_unique<TcpAcceptor>(loop, listenAddr))
{
}

void TcpServer::start()
{
    if (loop_->isInLoopThread())
    {
        startInLoop();
    }
    else
    {
        loop_->queueInLoop([this] {
            startInLoop();
        });
    }
}

void TcpServer::startInLoop()
{
    loop_->assertInLoopThread();

    acceptor_->setNewConnectionCallback(
        [this](int sockfd, const InetAddress& peerAddr) {
            newConnection(sockfd, peerAddr);
    });
    acceptor_->listen();
}

void TcpServer::stop()
{
    loop_->runInLoop([this] {
        acceptor_.reset();
        {
            std::vector<TcpConnectionPtr>
                tempConn(connections_.begin(), connections_.end());
            for (const auto& conn : tempConn)
            {
                conn->forceClose();
            }
        }
    });
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
    loop_->assertInLoopThread();

    EventLoop* ioLoop = loop_;
    if (loopPool_)
    {
        ioLoop = loopPool_->getNextLoop();
    }
    auto conn = std::make_shared<TcpConnection>(ioLoop, sockfd, peerAddr);
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    if (loop_->isInLoopThread())
    {
        removeConnectionInLoop(conn);
    }
    else
    {
        loop_->queueInLoop([this, conn] {
            removeConnectionInLoop(conn);
        });
    }
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
    loop_->assertInLoopThread();

    [[maybe_unused]] size_t n = connections_.erase(conn);
    assert(n == 1);
}

}