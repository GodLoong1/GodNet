#include "godnet/util/logger.hpp"

int main()
{
    godnet::Logger::Instance().setLevel(spdlog::level::trace);
    GODNET_LOG_DEBUG("this is {}", "debug");
    GODNET_LOG_CRITICAL("wdf");
}