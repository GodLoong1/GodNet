#ifndef GODNET_NETWORK_ACCEPTOR_HPP
#define GODNET_NETWORK_ACCEPTOR_HPP

#include "godnet/config.hpp"
#include "godnet/util/noncopyable.hpp"
#include "godnet/network/socket.hpp"
#include "godnet/network/inet_address.hpp"

#include <functional>
#include <memory>

namespace godnet
{

class EventLoop;
class EventChannel;

class GODNET_EXPORT Acceptor : Noncopyable
{
public:
    using NewConnectionCallback = std::function<void(int sockfd, const InetAddress&)>;
    using AcceptorSockOptCallback = std::function<void(int sockfd)>;

    Acceptor(EventLoop* loop,
             const InetAddress& addr,
             bool reuseAddr = true,
             bool reusePort = true);
    ~Acceptor();

private:
    void handleRead();

    EventLoop* const loop_;
    const InetAddress addr_;
    Socket socket_;
    std::unique_ptr<EventChannel> accpet_channel_;
    NewConnectionCallback newConnectionCallback_;
#if defined(GODNET_LINUX)
    int idleFd_{};
#endif
};

}

#endif
