#ifndef GODNET_UTIL_NONCOPYABLE_HPP
#define GODNET_UTIL_NONCOPYABLE_HPP

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
