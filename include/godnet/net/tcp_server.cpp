#include "godnet/net/tcp_server.hpp"
#include "godnet/net/tcp_acceptor.hpp"

#include <cassert>

namespace godnet
{

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr)
: loop_(loop),
  acceptor_(std::make_unique<TcpAcceptor>(loop, listenAddr))
{
}

TcpServer::~TcpServer()
{
    if (loopPool_)
    {
        loopPool_->stop();
    }
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
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallback([this](const TcpConnectionPtr& conn) {
        removeConnection(conn);
    });

    conn->init();
    connections_.emplace(std::move(conn));
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