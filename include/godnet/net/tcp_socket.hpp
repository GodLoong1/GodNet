#ifndef GODNET_NET_TCP_SOCKET_HPP
#define GODNET_NET_TCP_SOCKET_HPP

#include "godnet/util/noncopyable.hpp"
#include "godnet/net/inet_address.hpp"

namespace godnet
{

class TcpSocket : Noncopyable
{
public:
    explicit TcpSocket(int sockfd);
    ~TcpSocket();

    // 绑定地址
    bool bind(const InetAddress& localAddr);

    // 启动监听
    bool listen();

    // 获取连接
    int accept(InetAddress& peerAddr);

    // 发起连接
    void connect(const InetAddress& peerAddr);

    // 关闭写端
    bool shutdown();

    // 强制关闭
    bool forceClose();

    // 读取数据
    std::int64_t read(char* buf[2], std::size_t len[2], std::size_t count);

    // 写入数据
    std::int64_t write(const char* buf, std::size_t len);

    // 获取本端地址
    InetAddress getLocalAddr();

    // 获取对端地址
    InetAddress getPeerAddr();

    // 设置Nagle算法
    bool setTcpNoDelay(bool on);

    // 设置重用地址
    bool setReuseAddr(bool on);

    // 设置重用监听端口
    bool setReusePort(bool on);

    // 设置Tcp心跳
    bool setKeepAlive(bool on);

    int fd() const noexcept
    {
        return sockfd_;
    }

private:
    int sockfd_{};
};

}

#endif
