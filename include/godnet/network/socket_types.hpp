#ifndef GODNET_NETWORK_SOCKET_TYPES_HPP
#define GODNET_NETWORK_SOCKET_TYPES_HPP

#pragma once

#if defined(GODNET_WIN)
    #include <WinSock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#endif

namespace godnet
{

#if defined(GODNET_WIN)
    using socket_t = ::SOCKET;
    using socklen_t = int;
#else
    using socket_t = int;
    using socklen_t = socklen_t;
    constexpr socket_t INVALID_SOCKET = -1;
#endif

using sockaddr_t = struct ::sockaddr;
using sockaddr_in4_t = struct ::sockaddr_in;
using sockaddr_in6_t = struct ::sockaddr_in6;

}

#endif
