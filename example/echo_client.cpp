#include "godnet/net/event_loop.hpp"
#include "godnet/net/inet_address.hpp"
#include "godnet/net/tcp_client.hpp"
#include "godnet/net/tcp_connection.hpp"
#include "godnet/util/logger.hpp"
#include <thread>
#include <iostream>

int main()
{
    godnet::EventLoop loop;
    godnet::InetAddress peerAddr(godnet::InetAddress::V4Any(9981));
    godnet::TcpClient client(&loop, peerAddr);
    client.setConnectionCallback([] (const godnet::TcpConnectionPtr& conn) {
        if (conn->isConnected())
        {
            GODNET_LOG_INFO("connected fd: {}, localAddr: {}, peerAddr: {}",
                conn->getFd(), conn->getLocalAddr().toIpPort(), conn->getPeerAddr().toIpPort());

            std::thread([connWeak = conn->weak_from_this()] {
                std::string line;
                while (std::getline(std::cin, line))
                {
                    line += '\n';
                    if (auto conn = connWeak.lock())
                    {
                        conn->send(line);
                    }
                    else
                    {
                        break;
                    }
                }
            }).detach();
        }
        else if (conn->isDisconnected())
        {
            GODNET_LOG_INFO("disconnected fd: {}, localAddr: {}, peerAddr: {}",
                conn->getFd(), conn->getLocalAddr().toIpPort(), conn->getPeerAddr().toIpPort());
        }
    });
    client.setMessageCallback([](const godnet::TcpConnectionPtr& conn, godnet::MessageBuffer& msg) {
        std::string str = msg.readAll();
        str.back() = '\0';
        GODNET_LOG_INFO("recv: {}", str);
        str += str;
        conn->send(str);
    });
    client.start();
    loop.start();
}