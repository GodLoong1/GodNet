#include "godnet/util/logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace godnet
{

Logger& Logger::Instance() noexcept
{
    static Logger logger;
    return logger;
}

Logger::Logger()
{
    log_ = spdlog::stdout_color_mt("godnet");
    log_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] [%s:%# %!] %v");
    log_->set_level(spdlog::level::trace);
}

}