#ifndef GODNET_UTIL_ENDIAN_HPP
#define GODNET_UTIL_ENDIAN_HPP

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <arpa/inet.h>
#endif

#include <cstdint>
#include <type_traits>

namespace godnet
{

namespace detail
{

template<typename T>
constexpr bool requiredEndianType = std::is_same_v<T, std::uint16_t> |
                                    std::is_same_v<T, std::uint32_t> |
                                    std::is_same_v<T, std::uint64_t>;

}

template<typename T, typename =
    std::enable_if_t<detail::requiredEndianType<T>>>
inline T hostToNetwork(T val) noexcept
{
    if constexpr (std::is_same_v<T, std::uint16_t>)
    {
        return ::htons(val);
    }
    else if constexpr (std::is_same_v<T, std::uint32_t>)
    {
        return ::htonl(val);
    }
    else if constexpr (std::is_same_v<T, std::uint64_t>)
    {
#ifdef _WIN32
        return (static_cast<std::uint64_t>(::htonl(val)) << 32) |
               ::htonl(val >> 32);
#else
        return ::htobe64(val);
#endif
    }
}

template<typename T, typename =
    std::enable_if_t<detail::requiredEndianType<T>>>
inline T networkToHost(T val) noexcept
{
    if constexpr (std::is_same_v<T, std::uint16_t>)
    {
        return ::ntohs(val);
    }
    else if constexpr (std::is_same_v<T, std::uint32_t>)
    {
        return ::ntohl(val);
    }
    else if constexpr (std::is_same_v<T, std::uint64_t>)
    {
#ifdef _WIN32
        return (static_cast<std::uint64_t>(::ntohl(val)) << 32) |
               ::ntohl(val >> 32);
#else
        return ::be64toh(val);
#endif
    }
}

}

#endif