#ifndef GODNET_UTIL_EXCEPTION_HPP
#define GODNET_UTIL_EXCEPTION_HPP

#include <stdexcept>
#include <system_error>

#include "fmt/core.h"

#include "godnet/util/system.hpp"

#define GODNET_THROW_RUNERR(...) \
    throw godnet::GodNetRunErr(__VA_ARGS__)

#define GODNET_THROW_SYSERR(...) \
    throw godnet::GodNetSysErr(__VA_ARGS__)

namespace godnet
{

class GodNetRunErr : public std::runtime_error
{
public:
    template <typename... Args>
    GodNetRunErr(const std::string& msg, Args&&... args)
    : std::runtime_error(fmt::format(msg, std::forward<Args>(args)...))
    {
    }
};

class GodNetSysErr : public std::system_error
{
public:
    template <typename... Args>
    GodNetSysErr(const std::string& msg, Args&&... args)
    : std::system_error(system::getSystemErrno(),
                        std::system_category(),
                        fmt::format(msg, std::forward<Args>(args)...))
    {
    }
};

}

#endif
