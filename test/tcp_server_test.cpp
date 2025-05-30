#include "godnet/net/inet_address.hpp"
#include "godnet/net/message_buffer.hpp"
#include "godnet/net/tcp_connection.hpp"
#include "godnet/net/tcp_server.hpp"

#include <iostream>

int main()
{
    godnet::EventLoop loop;
    godnet::TcpServer server(&loop, godnet::InetAddress::V4Any(9981));
    server.setMessageCallback([](const godnet::TcpConnectionPtr& conn, godnet::MessageBuffer& buf) {
        std::cout << buf.readAll() << std::endl;
    });
    server.setConnectionCallback([](const godnet::TcpConnectionPtr& conn) {
        std::cout << conn->getPeerAddr().toIpPort() << std::endl;
    });
    server.start();
    loop.start();
}