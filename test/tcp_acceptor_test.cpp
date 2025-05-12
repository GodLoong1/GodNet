#include "godnet/network/endpoint.hpp"
#include "godnet/network/tcp_acceptor.hpp"
#include "godnet/network/event_loop.hpp"
#include "godnet/network/event_poller.hpp"
#include "godnet/network/event_channel.hpp"
#include "fmt/core.h"

int main()
{
    godnet::EventLoop loop;
    auto addr{godnet::makeEndpointV4Any(8080)};
    godnet::TcpAcceptor acceptor(&loop, addr);
    acceptor.setNewConnectionCallback([](int fd, const godnet::Endpoint& endpoint) {
        fmt::print("New connection fd {} from {}\n", fd, endpoint.toIpPort());
    });
    acceptor.listen();
    loop.start();
}