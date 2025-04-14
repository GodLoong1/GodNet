#ifndef GODNET_UTIL_SYSTEM_HPP
#define GODNET_UTIL_SYSTEM_HPP

#include "godnet/config.hpp"

#include <cstdint>

namespace godnet
{

GODNET_EXPORT std::uint64_t getThreadId() noexcept;

}

#endif
