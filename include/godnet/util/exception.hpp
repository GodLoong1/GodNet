#ifndef GODNET_UTIL_EXCEPTION_HPP
#define GODNET_UTIL_EXCEPTION_HPP

#include <stdexcept>
#include <system_error>

#include "fmt/core.h"

#include "godnet/util/system.hpp"

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

template<typename... Args>
void throwGodNetRunErr(const std::string& msg, Args&&... args) noexcept(false)
{
    throw godnet::GodNetRunErr(msg, std::forward<Args>(args)...);
}

template<typename... Args>
void throwGodNetSysErr(const std::string& msg, Args&&... args) noexcept(false)
{
    throw godnet::GodNetSysErr(msg, std::forward<Args>(args)...);
}

}

#endif
