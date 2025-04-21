#ifndef GODNET_UTIL_SYSTEM_HPP
#define GODNET_UTIL_SYSTEM_HPP

#include "godnet/config.hpp"

#include <cstdint>
#include <string>
#include <cstring>

namespace godnet::system
{

GODNET_EXPORT std::uint64_t getThreadId() noexcept;

GODNET_EXPORT int getSystemErrno() noexcept;

GODNET_EXPORT std::string getSystemErrorMessage(int err) noexcept;

GODNET_EXPORT std::string getStackTrace(int maxFrames = 10) noexcept;

}

#endif
