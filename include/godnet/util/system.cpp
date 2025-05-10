#include "godnet/util/system.hpp"

#include <cstring>
#include <cerrno>

#ifdef _WIN32
    #include <windows.h>
    #include <sys/timeb.h>
#else
    #include <unistd.h>
    #include <sys/syscall.h>
    #include <sys/time.h>
#endif

namespace godnet::system
{

std::uint64_t getThreadId() noexcept
{
    thread_local std::uint64_t thread_id =
#ifdef _WIN32
    static_cast<std::uint64_t>(::GetCurrentThreadId());
#else
    static_cast<std::uint64_t>(::syscall(SYS_gettid));
#endif
    return thread_id;
}

int getSystemErrno() noexcept
{
#ifdef _WIN32
    return ::WSAGetLastError();
#else
    return errno;
#endif
}

std::string getSystemErrorMessage(int err) noexcept
{
#ifdef _WIN32
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
#else
    return std::strerror(err);
#endif
}

}
