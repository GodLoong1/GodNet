#ifndef GODNET_UTIL_SYSTEM_HPP
#define GODNET_UTIL_SYSTEM_HPP

#include <cstdint>
#include <string>

namespace godnet::system
{

std::uint64_t getThreadId() noexcept;

int getSystemErrno() noexcept;

std::string getSystemErrorMessage(int err) noexcept;

void setThreadName(const std::string& name) noexcept;

}

#endif
