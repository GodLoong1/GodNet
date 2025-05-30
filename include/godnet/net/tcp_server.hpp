#ifndef GODNET_NETWORK_TCP_SERVER_HPP
#define GODNET_NETWORK_TCP_SERVER_HPP

#include <set>

#include "godnet/util/noncopyable.hpp"
#include "godnet/net/tcp_acceptor.hpp"
#include "godnet/net/tcp_connection.hpp"
#include "godnet/net/event_loop.hpp"
#include "godnet/net/event_loop_thread_pool.hpp"
#include "godnet/net/inet_address.hpp"

namespace godnet
{

class TcpServer : Noncopyable
{
public:
    TcpServer(EventLoop* loop, const InetAddress& listenAddr);
    ~TcpServer();

    void start();
    void startInLoop();
    void stop();

    void setConnectionCallback(TcpConnectionCallback&& callback) noexcept
    {
        connectionCallback_ = std::move(callback);
    }

    void setMessageCallback(TcpMessageCallback&& callback) noexcept
    {
        messageCallback_ = std::move(callback);
    }

private:
    void newConnection(int sockfd, const InetAddress& peerAddr);
    void removeConnection(const TcpConnectionPtr& conn);
    void removeConnectionInLoop(const TcpConnectionPtr& conn);

    EventLoop* loop_;
    std::unique_ptr<TcpAcceptor> acceptor_;
    std::unique_ptr<EventLoopThreadPool> loopPool_;

    std::set<TcpConnectionPtr> connections_;
    TcpConnectionCallback connectionCallback_;
    TcpMessageCallback messageCallback_;
};

}

#endif