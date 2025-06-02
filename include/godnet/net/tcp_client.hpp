#ifndef GODNET_NET_TCP_CLIENT_HPP
#define GODNET_NET_TCP_CLIENT_HPP

#include <mutex>

#include "godnet/util/noncopyable.hpp"
#include "godnet/net/event_loop.hpp"
#include "godnet/net/inet_address.hpp"
#include "godnet/net/tcp_connector.hpp"
#include "godnet/net/tcp_connection.hpp"

namespace godnet
{

class TcpClient : Noncopyable
{
public:
    TcpClient(EventLoop* loop,
              const InetAddress& serverAddr) noexcept;
    ~TcpClient();

    void start() noexcept;
    void stop() noexcept;

    void setConnectionCallback(TcpConnectionCallback&& cb) noexcept
    {
        connectionCallback_ = std::move(cb);
    }

    void setMessageCallback(TcpMessageCallback&& cb) noexcept
    {
        messageCallback_ = std::move(cb);
    }

    void setRetry(bool on) noexcept
    {
        retry_ = on;
    }

private:
    void newConnection(int sockfd) noexcept;
    void removeConnection(const TcpConnectionPtr& conn) noexcept;

    EventLoop* loop_;
    std::unique_ptr<TcpConnector> connector_;
    bool started_{false};
    bool retry_{false};

    TcpConnectionCallback connectionCallback_;
    TcpMessageCallback messageCallback_;
    TcpConnectionPtr connection_;
};

} // namespace god

#endif