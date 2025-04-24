#ifndef GODNET_NETWORK_TCP_ACCEPTOR_HPP
#define GODNET_NETWORK_TCP_ACCEPTOR_HPP

#include "godnet/config.hpp"
#include "godnet/util/noncopyable.hpp"
#include "godnet/network/tcp_socket.hpp"
#include "godnet/network/inet_address.hpp"

#include <functional>
#include <memory>

namespace godnet
{

class EventLoop;
class EventChannel;

class GODNET_EXPORT TcpAcceptor : Noncopyable
{
public:
    using NewConnectionCallback = std::function<void(TcpSocket, const InetAddress&)>;
    using AcceptorSockOptCallback = std::function<void(TcpSocket)>;

    TcpAcceptor(EventLoop* loop,
                const InetAddress& addr,
                bool reuseAddr = true,
                bool reusePort = true);
    ~TcpAcceptor();

    void listen();

private:
    void handleRead();

    EventLoop* const loop_;
    const InetAddress addr_;
    TcpSocket socket_;
    std::unique_ptr<EventChannel> channel_;
    NewConnectionCallback newConnectionCallback_;
#if defined(GODNET_LINUX)
    int idleFd_{};
#endif
};

}

#endif
