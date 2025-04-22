#include "godnet/util/debug.hpp"

#include "godnet/util/system.hpp"

#include <fmt/format.h>
#include <system_error>

namespace godnet
{

const char* getFileName(const char* file)
{
#if defined(GODNET_WIN)
    if (const char* find = std::strrchr(file, '\\'))
#elif defined(GODNET_LINUX)
    if (const char* find = std::strrchr(file, '/'))
#endif
    {
        file = find + 1;
    }
    return file;
}

RuntimeError::RuntimeError(const char* file, int line,
                           const char* func, const char* msg)
: std::runtime_error(fmt::format("{}:{} {}: {}", getFileName(file), line, func, msg))
{
}

SystemError::SystemError(const char* file, int line,
                           const char* func, const char* msg)
: std::system_error(system::getSystemErrno(),
                    std::system_category(),
                    fmt::format("{}:{} {}: {}", getFileName(file), line, func, msg))
{
}

}