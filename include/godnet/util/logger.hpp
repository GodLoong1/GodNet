#ifndef GODNET_UTIL_LOGGER_HPP
#define GODNET_UTIL_LOGGER_HPP

#include "spdlog/spdlog.h"

#define GODNET_LOG(lv, ...) \
    if (godnet::Logger::Instance().getLevel() >= lv) \
        godnet::Logger::Instance().getLog()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, lv, __VA_ARGS__)

#define GODNET_LOG_TRACE(...) GODNET_LOG(spdlog::level::trace, __VA_ARGS__)
#define GODNET_LOG_DEBUG(...) GODNET_LOG(spdlog::level::debug, __VA_ARGS__)

namespace godnet
{

class Logger
{
public:
    static Logger& Instance() noexcept;

    const std::shared_ptr<spdlog::logger>& getLog() const noexcept
    {
        return log_;
    }

    spdlog::level_t getLevel() const noexcept
    {
        return log_->level();
    }

private:
    Logger();

    std::shared_ptr<spdlog::logger> log_{};
};

}

#endif
