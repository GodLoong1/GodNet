#ifndef GODNET_NETWORK_TCP_CONNECTION_HPP
#define GODNET_NETWORK_TCP_CONNECTION_HPP

#include <memory>
#include <functional>
#include <any>

#include "godnet/util/noncopyable.hpp"
#include "godnet/net/inet_address.hpp"
#include "godnet/net/message_buffer.hpp"
#include "godnet/net/tcp_socket.hpp"

namespace godnet
{

class EventLoop;
class EventChannel;
class TcpConnection;

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

using TcpConnectionCallback =
    std::function<void(const TcpConnectionPtr&)>;

using TcpMessageCallback =
    std::function<void(const TcpConnectionPtr&, MessageBuffer& buf)>;

using TcpCloseCallback = 
    std::function<void(const TcpConnectionPtr&)>;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>,
                      private Noncopyable
{
public:
    TcpConnection(EventLoop* loop,
                  int sockfd,
                  const InetAddress& peerAddr);
    ~TcpConnection();

    void init();
    void shutdown();
    void forceClose();

    void send(const char* buf, std::size_t len);
    void send(const std::string& buf);
    void send(std::string&& buf);
    void send(const MessageBuffer& buf);
    void sendInLoop(const char* buf, std::size_t len);

    void setConnectionCallback(const TcpConnectionCallback& callback)
    {
        connectionCallback_ = std::move(callback);
    }

    void setMessageCallback(const TcpMessageCallback& callback)
    {
        messageCallback_ = std::move(callback);
    }

    void setCloseCallback(TcpCloseCallback&& callback)
    {
        closeCallback_ = std::move(callback);
    }

    int getFd() const noexcept
    {
        return socket_.fd();
    }

    const InetAddress& getLocalAddr() const noexcept
    {
        return localAddr_;
    }

    const InetAddress& getPeerAddr() const noexcept
    {
        return peerAddr_;
    }

    bool isConnected() const noexcept
    {
        return status_ == Status::connected;
    }

    bool isDisconnected() const noexcept
    {
        return status_ == Status::disconnected;
    }

private:
    void initInLoop();
    void shutdownInLoop();
    void forceCloseInLoop();

    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();

private:
    enum class Status : std::uint8_t
    {
        disconnected,
        connecting,
        connected,
        disconnecting,
    };

    EventLoop* loop_;
    TcpSocket socket_;
    std::unique_ptr<EventChannel> channel_;
    Status status_{Status::disconnected};

    InetAddress localAddr_;
    InetAddress peerAddr_;

    TcpConnectionCallback connectionCallback_;
    TcpMessageCallback messageCallback_;
    TcpCloseCallback closeCallback_;

    MessageBuffer readBuffer_;
    MessageBuffer writeBuffer_;
    std::any context_;
};

}

#endif