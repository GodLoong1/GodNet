#ifndef GODNET_NETWORK_TCP_SOCKET_HPP
#define GODNET_NETWORK_TCP_SOCKET_HPP

#include "godnet/config.hpp"

#include "godnet/util/noncopyable.hpp"

namespace godnet
{

class InetAddress;

class GODNET_EXPORT TcpSocket : Noncopyable
{
public:
    TcpSocket() = default;
    TcpSocket(int sockfd);
    TcpSocket(TcpSocket&& other) noexcept;
    TcpSocket& operator=(TcpSocket&& other) noexcept;
    ~TcpSocket();

    void swap(TcpSocket& other) noexcept;

    static int CreateSocket(int family);

    void bind(const InetAddress& localaddr);
    void listen();
    TcpSocket accept(InetAddress& peeraddr);
    void closeWrite();
    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);

    int fd() const noexcept
    {
        return sockfd_;
    }

private:
    int sockfd_{-1};
};

}

#endif
