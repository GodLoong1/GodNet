#ifndef GODNET_NETWORK_TCP_SERVER_HPP
#define GODNET_NETWORK_TCP_SERVER_HPP

#include "godnet/util/noncopyable.hpp"
#include "godnet/network/event_loop.hpp"
#include "godnet/network/endpoint.hpp"

namespace godnet
{

class TcpServer : Noncopyable
{
public:
    TcpServer(EventLoop* loop,
              const Endpoint& listen,
              bool reuseAddr = true,
              bool reusePort = true);

private:

};

}

#endif