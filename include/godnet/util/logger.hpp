#ifndef GODNET_UTIL_LOGGER_HPP
#define GODNET_UTIL_LOGGER_HPP

#include "spdlog/logger.h"

#include "godnet/util/noncopyable.hpp"
#include "godnet/util/system.hpp"

#define GODNET_LOG(lv, ...)\
    if (godnet::Logger::Instance().getLevel() <= spdlog::level::lv)     \
        godnet::Logger::Instance().getLog()->log(                       \
            spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},    \
            spdlog::level::lv,                                          \
            __VA_ARGS__)

#define GODNET_LOG_TRACE(...)       GODNET_LOG(trace, __VA_ARGS__)
#define GODNET_LOG_DEBUG(...)       GODNET_LOG(debug, __VA_ARGS__)
#define GODNET_LOG_INFO(...)        GODNET_LOG(info, __VA_ARGS__)
#define GODNET_LOG_WARN(...)        GODNET_LOG(warn, __VA_ARGS__)
#define GODNET_LOG_ERROR(...)       GODNET_LOG(err, __VA_ARGS__)
#define GODNET_LOG_CRITICAL(...)    GODNET_LOG(critical, __VA_ARGS__)

#define GODNET_LOG_SYSERR_1(msg) \
    GODNET_LOG(err, msg ", errno: {}", godnet::system::getErrnoMsg())

#define GODNET_LOG_SYSERR_N(msg, ...) \
    GODNET_LOG(err, msg ", errno: {}", __VA_ARGS__, godnet::system::getErrnoMsg())

#define GODNET_LOG_SYSERR_OVERLOAD( \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME

#define GODNET_LOG_SYSERR(...) \
    GODNET_LOG_SYSERR_OVERLOAD(__VA_ARGS__, \
        GODNET_LOG_SYSERR_N, \
        GODNET_LOG_SYSERR_N, \
        GODNET_LOG_SYSERR_N, \
        GODNET_LOG_SYSERR_N, \
        GODNET_LOG_SYSERR_N, \
        GODNET_LOG_SYSERR_N, \
        GODNET_LOG_SYSERR_N, \
        GODNET_LOG_SYSERR_N, \
        GODNET_LOG_SYSERR_N, \
        GODNET_LOG_SYSERR_N, \
        GODNET_LOG_SYSERR_1) (__VA_ARGS__)

namespace godnet
{

class Logger : Noncopyable
{
public:
    static Logger& Instance() noexcept;

    const std::shared_ptr<spdlog::logger>& getLog() const noexcept
    {
        return log_;
    }

    spdlog::level::level_enum getLevel() const noexcept
    {
        return log_->level();
    }

    void setLevel(spdlog::level::level_enum level) noexcept
    {
        log_->set_level(level);
    }

private:
    Logger();

    std::shared_ptr<spdlog::logger> log_{};
};

}

#endif
