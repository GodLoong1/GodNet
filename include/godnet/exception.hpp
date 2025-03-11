#ifndef GODNET_EXCEPTION_HPP
#define GODNET_EXCEPTION_HPP

#include "godnet/config.hpp"

#include <exception>
#include <string>

#define GODNET_THROW(msg) \
    throw godnet::GodNetException(__FILE__, __LINE__, GODNET_FUNCTION, msg)

namespace godnet
{

class GodNetException : public std::exception
{
public:
    GodNetException(const char* file, int line,
                    const char* func, const char* msg) noexcept;

    const char* what() const noexcept override
    {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

}

#endif
