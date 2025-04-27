#include "godnet/network/endpoint.hpp"
#include "godnet/network/tcp_acceptor.hpp"
#include "godnet/network/event_loop.hpp"
#include "godnet/network/event_channel.hpp"
#include "godnet/network/event_poller.hpp"
#include "fmt/core.h"

int main()
{
    godnet::EventLoop loop;
    auto addr{godnet::makeEndpointV4Any(8080)};
    godnet::TcpAcceptor acceptor(&loop, addr);
    acceptor.setNewConnectionCallback([](godnet::TcpSocket socket, const godnet::Endpoint& endpoint) {
        fmt::print("New connection from {}\n", endpoint.toIpPort());
    });
    acceptor.listen();
    loop.loop();
}