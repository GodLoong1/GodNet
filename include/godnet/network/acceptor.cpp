#include "godnet/network/acceptor.hpp"

namespace godnet
{

Acceptor::Acceptor(EventLoop* loop,
                   const InetAddress& addr,
                   bool reuseAddr,
                   bool reusePort)
: loop_(loop),
  addr_(addr)
{
    if (reuseAddr)
    {
        socket_.setReuseAddr(true);
    }
    if (reusePort)
    {
        socket_.setReusePort(true);
    }
    socket_.bind(addr_);
    accpet_channel_ = std::make_unique<EventChannel>(loop_, socket_.fd());
    accpet_channel_->setReadCallback([this]() { handleRead(); });

}

}