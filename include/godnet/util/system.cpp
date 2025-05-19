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
    #include <sys/prctl.h>
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

std::string getSystemErrnoMessage(int err) noexcept
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

void setThreadName(const std::string& name) noexcept
{
#ifdef __linux__
    ::prctl(PR_SET_NAME, name.c_str());
#else
    typedef HRESULT(WINAPI* SetThreadDescriptionFn)(HANDLE, PCWSTR);
    
    HMODULE hKernel32 = GetModuleHandleW(L"Kernel32.dll");
    auto setThreadDescription = reinterpret_cast<SetThreadDescriptionFn>(
        GetProcAddress(hKernel32, "SetThreadDescription")
    );
    if (setThreadDescription)
    {
        int wlen = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, nullptr, 0);
        std::wstring wname(wlen, 0);
        MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, &wname[0], wlen);
        setThreadDescription(getThreadId(), wname.c_str());
    }
#endif
}

}
