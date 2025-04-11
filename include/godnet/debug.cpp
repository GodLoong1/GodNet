#include "godnet/debug.hpp"

#include <cstring>
#include <fmt/format.h>

namespace godnet
{

GodNetException::GodNetException(const char* file, int line,
                                 const char* func, const char* msg) noexcept
    : std::exception()
{
#if defined(GODNET_WIN)
    file = std::strrchr(file, '\\') + 1;
#elif defined(GODNET_LINUX)
    file = std::strrchr(file, '/') + 1;
#endif
    msg_ = fmt::format("{}:{}:{} - {}", file, line, func, msg);
}

}