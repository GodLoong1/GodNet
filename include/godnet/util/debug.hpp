#ifndef GODNET_EXCEPTION_HPP
#define GODNET_EXCEPTION_HPP

#include "godnet/config.hpp"

#include <system_error>
#include <cassert>

#define GODNET_THROW_RUNERR(msg) \
    throw godnet::RuntimeError(__FILE__, __LINE__, GODNET_FUNCTION, msg)

#define GODNET_THROW_SYSERR(msg) \
    throw godnet::SystemError(__FILE__, __LINE__, GODNET_FUNCTION, msg)

#ifndef GODNET_NDEBUG
    #define GODNET_ASSERT(expr) \
        (static_cast<bool>(expr) \
        ? static_cast<void>(0) \
        : __assert_fail (#expr, __FILE__, __LINE__, GODNET_FUNCTION))
#else
    #define GODNET_ASSERT(expr) static_cast<void>(0)
#endif

namespace godnet
{

class GODNET_EXPORT RuntimeError : public std::runtime_error
{
public:
    RuntimeError(const char* file, int line, const char* func, const char* msg);
};

class GODNET_EXPORT SystemError : public std::system_error
{
public:
    SystemError(const char* file, int line, const char* func, const char* msg);
};

}

#endif
