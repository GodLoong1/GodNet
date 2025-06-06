#include "godnet/util/system.hpp"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/syscall.h>
    #include <cstring>
    #include <cerrno>
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

int getErrno() noexcept
{
#ifdef _WIN32
    return ::WSAGetLastError();
#else
    return errno;
#endif
}

std::string getErrnoMsg(int err) noexcept
{
#ifdef _WIN32
    char buffer[256];
    int len = ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM |
                               FORMAT_MESSAGE_IGNORE_INSERTS |
                               FORMAT_MESSAGE_MAX_WIDTH_MASK,
                               nullptr,
                               err,
                               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                               buffer,
                               sizeof(buffer);
                               nullptr);
    return std::string(buffer, len);
#else
    return std::strerror(err);
#endif
}

}
