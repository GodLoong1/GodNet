#ifndef GODNET_UTIL_LOGGER_HPP
#define GODNET_UTIL_LOGGER_HPP

#include "spdlog/logger.h"

#include "godnet/util/noncopyable.hpp"
#include "godnet/util/system.hpp"

#define GODNET_LOG(lv, msg, ...) \
    if (godnet::Logger::Instance().getLevel() <= lv) \
        godnet::Logger::Instance().getLog()->log( \
            spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, \
            lv, msg, ##__VA_ARGS__)

#define GODNET_LOG_TRACE(...) \
    GODNET_LOG(spdlog::level::trace, __VA_ARGS__)

#define GODNET_LOG_DEBUG(...) \
    GODNET_LOG(spdlog::level::debug, __VA_ARGS__)

#define GODNET_LOG_INFO(...) \
    GODNET_LOG(spdlog::level::info, __VA_ARGS__)

#define GODNET_LOG_WARN(...) \
    GODNET_LOG(spdlog::level::warn, __VA_ARGS__)

#define GODNET_LOG_ERROR(...) \
    GODNET_LOG(spdlog::level::err, __VA_ARGS__)

#define GODNET_LOG_SYSERR(msg, ...) \
    GODNET_LOG(spdlog::level::err, \
               msg ", errno: {}", \
               ##__VA_ARGS__, \
               godnet::system::getErrnoMsg())

#define GODNET_LOG_CRITICAL(...) \
    GODNET_LOG(spdlog::level::critical, __VA_ARGS__)

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
