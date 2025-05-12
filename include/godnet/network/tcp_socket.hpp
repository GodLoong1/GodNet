#ifndef GODNET_NETWORK_TCP_SOCKET_HPP
#define GODNET_NETWORK_TCP_SOCKET_HPP

#include "godnet/network/endpoint.hpp"
#include "godnet/util/noncopyable.hpp"

namespace godnet
{

class TcpSocket : Noncopyable
{
public:
    explicit TcpSocket(int sockfd) noexcept;
    ~TcpSocket();

    void bind(const Endpoint& localEndpoint);
    void listen();
    int accept(Endpoint& peerEndpoint);
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
