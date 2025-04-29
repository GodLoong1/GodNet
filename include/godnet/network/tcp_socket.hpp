#ifndef GODNET_NETWORK_TCP_SOCKET_HPP
#define GODNET_NETWORK_TCP_SOCKET_HPP

#include "godnet/config.hpp"

#include "godnet/network/endpoint.hpp"
#include "godnet/util/noncopyable.hpp"

namespace godnet
{

class GODNET_EXPORT TcpSocket : Noncopyable
{
public:
    static TcpSocket MakeSocket(int family);

    TcpSocket() = default;
    explicit TcpSocket(int sockfd);
    TcpSocket(TcpSocket&& other) noexcept;
    TcpSocket& operator=(TcpSocket&& other) noexcept;
    ~TcpSocket();

    void bind(const Endpoint& localEndpoint);
    void listen();
    TcpSocket accept(Endpoint& peerEndpoint);
    void closeWrite();

    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);

    int getSockfd() const noexcept
    {
        return sockfd_;
    }

private:
    int sockfd_{-1};
};

}

#endif
