#ifndef GODNET_EXCEPTION_HPP
#define GODNET_EXCEPTION_HPP

#pragma once

#include <exception>
#include <string>
#include <string_view>

#define GODNET_THROW(msg) \
    throw godnet::GodNetException(__FILE__, __LINE__, __FUNCTION__, msg)

namespace godnet
{

class GodNetException : public std::exception
{
public:
    GodNetException(std::string_view file, int line, std::string_view func, std::string_view msg)
    {
        msg_ += std::string(file);
        msg_ += ":";
        msg_ += std::to_string(line);
        msg_ += ":";
        msg_ += std::string(func);
        msg_ += ": ";
        msg_ += std::string(msg);
    }

    const char* what() const noexcept override
    {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

}

#endif
