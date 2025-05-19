#ifndef GODNET_NET_TCP_ACCEPTOR_HPP
#define GODNET_NET_TCP_ACCEPTOR_HPP

#include <functional>
#include <memory>

#include "godnet/util/noncopyable.hpp"
#include "godnet/net/inet_address.hpp"

namespace godnet
{

class EventLoop;
class EventChannel;
class TcpSocket;

class TcpAcceptor : Noncopyable
{
public:
    using NewConnectionCallback = std::function<void(int, const InetAddress&)>;

    TcpAcceptor(EventLoop* loop, const InetAddress& localAddr) noexcept;
    ~TcpAcceptor();

    bool listen() noexcept;
    bool setReuseAddr(bool on) noexcept;
    bool setReusePort(bool on) noexcept;

    void setNewConnectionCallback(NewConnectionCallback&& callback) noexcept
    {
        newConnectionCallback_ = std::move(callback);
    }

private:
    void handleRead();

    EventLoop* loop_;
    InetAddress localAddr_;
    std::unique_ptr<TcpSocket> socket_;
    std::unique_ptr<EventChannel> channel_;
    NewConnectionCallback newConnectionCallback_;
};

}

#endif
