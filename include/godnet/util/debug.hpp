#ifndef GODNET_EXCEPTION_HPP
#define GODNET_EXCEPTION_HPP

#include "godnet/config.hpp"

#include <exception>
#include <string>
#include <cassert>
#include <system_error>

#define GODNET_THROW(msg) \
    throw godnet::GodNetException(__FILE__, __LINE__, GODNET_FUNCTION, msg)

#define GODNET_THROW_IF(expr, msg) \
    static_cast<bool>(!(expr)) ? \
    static_cast<void>(0) : \
    GODNET_THROW(msg);

#ifndef GODNET_NDEBUG
    #define GODNET_ASSERT(expr) \
        static_cast<bool>(expr) ? \
        static_cast<void>(0) : \
        __assert_fail(#expr, __FILE__, __LINE__, GODNET_FUNCTION)
#else
    #define GODNET_ASSERT(expr) static_cast<void>(expr)
#endif

namespace godnet
{

class GODNET_EXPORT GodNetException : public std::exception
{
public:
    GodNetException(const char* file, int line,
                    const char* func, const char* msg) noexcept;

    const char* what() const noexcept override
    {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

}

#endif
