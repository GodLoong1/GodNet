#include "godnet/exception.hpp"

#include <cstring>

namespace godnet
{

GodNetException::GodNetException(const char* file, int line,
                                 const char* func, const char* msg) noexcept
{
#if defined(GODNET_LINUX)
    file = ::strrchr(file, '/') + 1;
#elif defined(GODNET_WIN)
    file = ::strrchr(file, '\\') + 1;
#endif
    msg_.append("[")
        .append(file)
        .append(":")
        .append(std::to_string(line))
        .append(" - ")
        .append(func)
        .append("]: ")
        .append(msg);
}

}