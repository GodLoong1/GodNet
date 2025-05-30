#ifndef GODNET_UTIL_SYSTEM_HPP
#define GODNET_UTIL_SYSTEM_HPP

#include <cstdint>
#include <string>

namespace godnet::system
{

std::uint64_t getThreadId() noexcept;

int getSystemErrno() noexcept;

std::string getSystemErrnoMessage(int err = getSystemErrno()) noexcept;

}

#endif
