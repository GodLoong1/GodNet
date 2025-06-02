#include "godnet/net/event_loop.hpp"
#include "godnet/net/inet_address.hpp"
#include "godnet/net/message_buffer.hpp"
#include "godnet/net/tcp_connection.hpp"
#include "godnet/net/tcp_server.hpp"
#include "godnet/util/logger.hpp"

int main()
{
    godnet::EventLoop loop;
    godnet::InetAddress listenAddr(godnet::InetAddress::V4Any(9981));
    godnet::TcpServer server(&loop, listenAddr);
    server.setConnectionCallback([](const godnet::TcpConnectionPtr& conn) {
        if (conn->isConnected())
        {
            GODNET_LOG_INFO("connected fd: {}, localAddr: {}, peerAddr: {}",
                conn->getFd(), conn->getLocalAddr().toIpPort(), conn->getPeerAddr().toIpPort());
        }
        else if (conn->isDisconnected())
        {
            GODNET_LOG_INFO("disconnected fd: {}, localAddr: {}, peerAddr: {}",
                conn->getFd(), conn->getLocalAddr().toIpPort(), conn->getPeerAddr().toIpPort());
        }
    });
    server.setMessageCallback([](const godnet::TcpConnectionPtr& conn, godnet::MessageBuffer& msg) {
        auto str = msg.readAll();
        str.back() = '\0';
        GODNET_LOG_INFO("fd: {}, {}", conn->getFd(), str.c_str());
        str.back() = '\n';
        conn->send(str);
    });
    server.start();
    loop.start();
}