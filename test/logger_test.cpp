#include "godnet/util/logger.hpp"

int main()
{
    godnet::Logger::Instance().setLevel(spdlog::level::debug);
    GODNET_LOG_DEBUG("this is {}", "debug");
}