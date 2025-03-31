#ifndef GODNET_EXCEPTION_HPP
#define GODNET_EXCEPTION_HPP

#include "godnet/config.hpp"

#include <exception>
#include <string>
#include <cstring>
#include <fmt/format.h>

#define GODNET_THROW(msg, ...) \
    throw godnet::GodNetException(__FILE__, __LINE__, GODNET_FUNCTION, "{}:{}:{}: " msg, ##__VA_ARGS__)

namespace godnet
{

class GodNetException : public std::exception
{
public:
    template <typename... Args>
    GodNetException(const char* file, int line,
                    const char* func, const char* msg, Args&&... args) noexcept
    {
#if defined(GODNET_WIN)
        file = std::strrchr(file, '\\') + 1;
#elif defined(GODNET_LINUX)
        file = std::strrchr(file, '/') +1;
#endif
        try
        {
            msg_ = fmt::format(msg, file, line, func, std::forward<Args>(args)...);
        }
        catch (std::exception& e)
        {
            msg_ = fmt::format("{}:{}:{}: {}", file, line, func, e.what());
        }
    }

    const char* what() const noexcept override
    {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

}

#endif
