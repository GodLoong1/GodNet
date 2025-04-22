#include "godnet/util/system.hpp"

#include <cstring>
#include <cerrno>

#if defined(GODNET_WIN)
    #include <windows.h>
#elif defined(GODNET_LINUX)
    #include <unistd.h>
    #include <sys/syscall.h>
#endif

namespace godnet::system
{

std::uint64_t getThreadId() noexcept
{
    thread_local std::uint64_t thread_id =
#if defined(GODNET_WIN)
    static_cast<std::uint64_t>(::GetCurrentThreadId());
#elif defined(GODNET_LINUX)
    static_cast<std::uint64_t>(::syscall(SYS_gettid));
#endif
    return thread_id;
}

int getSystemErrno() noexcept
{
#if defined(GODNET_WIN)
    return ::WSAGetLastError();
#elif defined(GODNET_LINUX)
    return errno;
#endif
}

std::string getSystemErrorMessage(int err) noexcept
{
#if defined(GODNET_WIN)
    char buffer[256];
    ::FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        buffer,
        sizeof(buffer),
        nullptr);
    return std::string(buffer);
#elif defined(GODNET_LINUX) 
    return std::strerror(err);
#endif
}

}
