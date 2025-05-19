#ifndef GODNET_NETWORK_TCP_CONNECTION_HPP
#define GODNET_NETWORK_TCP_CONNECTION_HPP

#include <memory>
#include <functional>

#include "godnet/network/event_channel.hpp"
#include "godnet/network/message_buffer.hpp"
#include "godnet/network/tcp_socket.hpp"
#include "godnet/util/noncopyable.hpp"

namespace godnet
{

class EventLoop;
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
                  const Endpoint& localEndpoint,
                  const Endpoint& peerEndpoint);
    ~TcpConnection();

private:
    enum class Status : std::uint8_t
    {
        connecting,
        connected,
        disconnecting,
        disconnected,
    };

    EventLoop* loop_;
    TcpSocket socket_;
    std::unique_ptr<EventChannel> channel_;
    Endpoint localEndpoint_;
    Endpoint peerEndpoint_;
    Status status_{Status::disconnected};

    TcpConnectionCallback connectionCallback_{};
    TcpMessageCallback messageCallback_{};
    TcpCloseCallback closeCallback_{};
};

}

#endif