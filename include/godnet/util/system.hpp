#ifndef GODNET_UTIL_SYSTEM_HPP
#define GODNET_UTIL_SYSTEM_HPP

#include "godnet/config.hpp"

#include <cstdint>

namespace godnet::system
{

GODNET_EXPORT std::uint64_t getThreadId() noexcept;

GODNET_EXPORT int getSystemErrno() noexcept;

}

#endif
