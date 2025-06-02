#include "godnet/net/tcp_client.hpp"
#include "godnet/net/inet_address.hpp"
#include "godnet/net/socket.hpp"

#include <cassert>
#include <future>

namespace godnet
{

TcpClient::TcpClient(EventLoop* loop,
                     const InetAddress& serverAddr) noexcept
: loop_(loop),
  connector_(std::make_unique<TcpConnector>(loop, serverAddr))
{
    assert(loop);
}

TcpClient::~TcpClient()
{
    loop_->assertInLoopThread();
    connector_.reset();
}

void TcpClient::start() noexcept
{
    loop_->runInLoop([this] {
        started_ = true;

        connector_->setNewConnectionCallback([this](int sockfd) {
            newConnection(sockfd);
        });
        connector_->connect();
    });
}

void TcpClient::stop() noexcept
{
    std::promise<void> promise;
    std::future<void> future = promise.get_future();

    loop_->runInLoop([this, &promise] {
        started_ = false;

        connector_.reset();
        if (connection_)
        {
            connection_->forceClose();
        }

        promise.set_value();
    });
    future.get();
}

void TcpClient::newConnection(int sockfd) noexcept
{
    loop_->assertInLoopThread();

    InetAddress peerAddr;
    socket::getPeerAddr(sockfd, peerAddr);
    connection_ = std::make_shared<TcpConnection>(loop_, sockfd, peerAddr);
    
    connection_->setConnectionCallback(connectionCallback_);
    connection_->setMessageCallback(messageCallback_);
    connection_->setCloseCallback([this](const TcpConnectionPtr& conn) {
        removeConnection(conn);
    });
    connection_->init();
}

void TcpClient::removeConnection(const TcpConnectionPtr&) noexcept
{
    loop_->assertInLoopThread();

    connection_.reset();
    if (retry_ && started_)
    {
        connector_->reconnect();
    }
}

}