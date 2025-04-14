#include "godnet/util/system.hpp"

#if defined(GODNET_WIN)
    #include <windows.h>
#elif defined(GODNET_LINUX)
    #include <unistd.h>
    #include <sys/syscall.h>
#endif

namespace godnet
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

}
