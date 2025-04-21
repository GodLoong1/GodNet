#include "godnet/util/system.hpp"

#include <sstream>

#if defined(GODNET_WIN)
    #include <windows.h>
#elif defined(GODNET_LINUX)
    #include <unistd.h>
    #include <sys/syscall.h>
    #include <errno.h>
    #include <execinfo.h>
    #include <cxxabi.h>
    #include <dlfcn.h>
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

std::string getStackTrace(int maxFrames) noexcept
{
    std::ostringstream oss;
#if defined(GODNET_WIN)
    // Windows实现
    HANDLE process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);
    SymSetOptions(SYMOPT_LOAD_LINES);

    void* stack[maxFrames];
    WORD frames = CaptureStackBackTrace(0, maxFrames, stack, NULL);

    SYMBOL_INFO* symbol = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char));
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    for (WORD i = 0; i < frames; i++) {
        SymFromAddr(process, (DWORD64)stack[i], 0, symbol);
        DWORD displacement;
        IMAGEHLP_LINE64 line;
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
        
        if (SymGetLineFromAddr64(process, (DWORD64)stack[i], &displacement, &line)) {
            oss << "[" << i << "] " << symbol->Name << " at " << line.FileName << ":" << line.LineNumber << "\n";
        } else {
            oss << "[" << i << "] " << symbol->Name << " at 0x" << (void*)stack[i] << "\n";
        }
    }

    free(symbol);
    SymCleanup(process);

#elif defined(GODNET_LINUX)
    void** callstack = new void*[maxFrames]{};
    int frames = backtrace(callstack, maxFrames);
    char** symbols = backtrace_symbols(callstack, frames);
    for (int i = 1; i < frames; i++)
    {
        oss << "[" << i-1 << "] " << symbols[i] << "\n";
    }

    free(symbols);
#endif

    return oss.str();
}

}
