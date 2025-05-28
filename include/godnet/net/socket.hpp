#ifndef GODNET_NET_SOCKET_HPP
#define GODNET_NET_SOCKET_HPP

#include <cstdint>

#include "godnet/net/inet_address.hpp"

namespace godnet::socket
{

// 设置非阻塞
void setNonBlock(int sockfd) noexcept;

// 创建Tcp套接字
int tcpSocket(int family) noexcept;

// 创建Udp套接字
int udpSocket(int family) noexcept;

// 关闭套接字
int closeSocket(int sockfd) noexcept;

// 绑定地址
int bind(int sockfd, const InetAddress& localAddr) noexcept;

// 监听地址
int listen(int sockfd) noexcept;

// 获取连接
int accept(int sockfd, InetAddress& peerAddr) noexcept;

// 发起连接
int connect(int sockfd, const InetAddress& localAddr) noexcept;

// Tcp读
std::int64_t read(int sockfd, char* buf[2], std::size_t len[2],
                  std::size_t count);

// Tcp写
std::int64_t write(int sockfd, const char* buf, std::size_t len);

// 获取本端地址
int getLocalAddr(int sockfd, InetAddress& localAddr) noexcept;

// 获取对端地址
int getPeerAddr(int sockfd, InetAddress& peerAddr) noexcept;

// 关闭写端
int shutdown(int sockfd) noexcept;

// 设置Nagle算法
int setTcpNoDelay(int sockfd, bool on) noexcept;

// 设置重用地址
int setReuseAddr(int sockfd, bool on) noexcept;

// 设置重用端口(Linux才有用)
int setReusePort(int sockfd, bool on) noexcept;

// 设置Tcp心跳
int setKeepAlive(int sockfd, bool on) noexcept;

// 获取套接字错误
int getSocketError(int sockfd) noexcept;

}

#endif