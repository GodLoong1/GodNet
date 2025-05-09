#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/async.h"
#include <memory>

int main()
{
    // 创建控制台彩色输出 sink
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);

    // 创建文件 sink（单一日志文件）
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/basic.log", true);
    file_sink->set_level(spdlog::level::debug);

    // 创建轮转文件 sink（5MB，保留 3 个文件）
    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/rotating.log", 1024 * 1024 * 5, 3);

    // 多 sink 合并成一个 logger
    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink, rotating_sink};
    auto logger = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(), sinks.end());

    // 设置为全局默认 logger（可选）
    spdlog::set_default_logger(logger);

    // 设置日志格式：[时间] [级别] 消息
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

    // 设置全局日志等级
    spdlog::set_level(spdlog::level::trace);

    // 各种日志级别示例
    SPDLOG_TRACE("This is a trace message");
    SPDLOG_DEBUG("This is a debug message");
    SPDLOG_INFO("Welcome to spdlog!");
    SPDLOG_WARN("This is a warning");
    SPDLOG_ERROR("This is an error");
    SPDLOG_CRITICAL("This is critical!");

    // 格式化日志
    int a = 42;
    spdlog::info("The answer is {}", a);

    // 支持 printf 风格
    spdlog::info("Printf style: %d", a);

    spdlog::init_thread_pool(8192, 1); // 队列大小，线程数
    auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "logs/async.txt");
    async_file->info("This is an async log message.");

    // 日志轮转测试
    for (int i = 0; i < 1000; ++i) {
        SPDLOG_INFO("Log message number {}", i);
    }

    spdlog::shutdown();
    return 0;
}
