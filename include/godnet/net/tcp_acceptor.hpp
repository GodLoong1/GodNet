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

    void listen();
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setNewConnectionCallback(NewConnectionCallback cb);

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
