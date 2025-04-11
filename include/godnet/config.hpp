#ifndef GODNET_CONFIG_HPP
#define GODNET_CONFIG_HPP

#if defined(_WIN32) || defined(_WIN64)
    #define GODNET_WIN
#elif defined(__linux__)
    #define GODNET_LINUX
#else
    #error "Unsupported platform"
#endif

#if defined(GODNET_WIN)
    // 构建动态库定义宏
    #if defined(GODNET_BUILD_SHARED)
        #define GODNET_EXPORT __declspec(dllexport)
    #else
        #define GODNET_EXPORT __declspec(dllimport)
    #endif
#elif defined(GODNET_LINUX)
    #define GODNET_EXPORT __attribute__((visibility("default")))
#endif

#if defined(GODNET_WIN)
    #define GODNET_FUNCTION __FUNCSIG__
#elif defined(GODNET_LINUX)
    #define GODNET_FUNCTION __PRETTY_FUNCTION__
#endif

namespace godnet
{

class Noncopyable
{
public:
    Noncopyable() noexcept = default;
    ~Noncopyable() noexcept = default;

    Noncopyable(const Noncopyable&) noexcept = delete;
    Noncopyable& operator=(const Noncopyable&) noexcept = delete;
};

}

#endif
