#include "godnet/network/tcp_connection.hpp"

#include "godnet/network/event_channel.hpp"

namespace godnet
{

TcpConnection::TcpConnection(EventLoop* loop,
                             int sockfd,
                             const Endpoint& localEndpoint,
                             const Endpoint& peerEndpoint)
: loop_(loop),
  socket_(sockfd),
  channel_(std::make_unique<EventChannel>(sockfd)),
  localEndpoint_(localEndpoint),
  peerEndpoint_(peerEndpoint)
{

}

TcpConnection::~TcpConnection()
{

}

}