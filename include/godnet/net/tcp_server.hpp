#ifndef GODNET_NETWORK_TCP_SERVER_HPP
#define GODNET_NETWORK_TCP_SERVER_HPP

#include "godnet/util/noncopyable.hpp"
#include "godnet/network/tcp_acceptor.hpp"
#include "godnet/network/event_loop.hpp"
#include "godnet/network/event_loop_thread_pool.hpp"
#include "godnet/network/endpoint.hpp"

namespace godnet
{

class TcpServer : Noncopyable
{
public:
    TcpServer(EventLoop* loop, const Endpoint& listen);

private:
    EventLoop* loop_{};
    std::unique_ptr<TcpAcceptor> acceptor_{};
    std::unique_ptr<EventLoopThreadPool> loopPool_;

    std::vector<EventLoop*> ioLoop_;
    std::size_t index_{};
};

}

#endif