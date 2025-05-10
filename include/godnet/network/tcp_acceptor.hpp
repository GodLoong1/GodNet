#ifndef GODNET_NETWORK_TCP_ACCEPTOR_HPP
#define GODNET_NETWORK_TCP_ACCEPTOR_HPP

#include <functional>
#include <memory>

#include "godnet/util/noncopyable.hpp"
#include "godnet/network/tcp_socket.hpp"
#include "godnet/network/endpoint.hpp"

namespace godnet
{

class EventLoop;
class EventChannel;

class TcpAcceptor : Noncopyable
{
public:
    using NewConnectionCallback = std::function<void(TcpSocket, const Endpoint&)>;

    TcpAcceptor(EventLoop* loop,
                const Endpoint& endpoint,
                bool reuseAddr = true,
                bool reusePort = true);
    ~TcpAcceptor();

    void listen();

    void setNewConnectionCallback(const NewConnectionCallback& cb)
    {
        newConnectionCallback_ = cb;
    }

    void setNewConnectionCallback(NewConnectionCallback&& cb) noexcept
    {
        newConnectionCallback_ = std::move(cb);
    }

private:
    void handleRead();

    EventLoop* loop_;
    Endpoint endpoint_;
    TcpSocket socket_;
    std::unique_ptr<EventChannel> channel_;
    NewConnectionCallback newConnectionCallback_;
};

}

#endif
