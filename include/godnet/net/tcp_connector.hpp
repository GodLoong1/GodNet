#ifndef GODNET_NET_TCP_CONNECTOR_HPP
#define GODNET_NET_TCP_CONNECTOR_HPP

#include <memory>
#include <functional>

#include "godnet/util/noncopyable.hpp"
#include "godnet/net/inet_address.hpp"
#include "godnet/net/timer.hpp"

namespace godnet
{

class EventLoop;
class EventChannel;

class TcpConnector : Noncopyable
{
public:
    using NewConnectionCallback = std::function<void(int sockfd)>;

    TcpConnector(EventLoop* loop,
                 const InetAddress& peerAddr,
                 TimerDuration retryDelay = 500ms,
                 TimerDuration maxDelay = 5s) noexcept;
    ~TcpConnector() noexcept;

    void setNewConnectionCallback(NewConnectionCallback&& cb) noexcept
    {
        newConnectionCallback_ = std::move(cb);
    }

    void connect() noexcept;
    void reconnect() noexcept;

private:
    void retry(int sockfd) noexcept;
    int resetChannel() noexcept;

    void handleWrite() noexcept;

    enum class Status : std::uint8_t
    {
        disconnected,
        connecting,
        connected,
    };

    EventLoop* const loop_;
    const InetAddress peerAddr_;
    const TimerDuration retryDelay_;
    TimerDuration currDelay_;
    const TimerDuration maxDelay_;

    Status status_{Status::disconnected};
    TimerId timerId_{0};
    std::shared_ptr<EventChannel> channel_;
    NewConnectionCallback newConnectionCallback_;
};

} // namespace god

#endif