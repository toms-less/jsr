#include <include/base.h>

base::LogConfig *base::Log::config_ = nullptr;

base::Log::Log(base::LogConfig *config)
{
    base::Log::config_ = config;
}

bool base::Log::initialize()
{
    std::shared_ptr<spdlog::logger> debug_logger = spdlog::rotating_logger_mt("debug.log", base::Log::config_->GetDebugLog(), 1048576 * base::Log::config_->GetMaxSize(), base::Log::config_->GetRotate());
    std::shared_ptr<spdlog::logger> server_logger = spdlog::rotating_logger_mt("server.log", base::Log::config_->GetServerLog(), 1048576 * base::Log::config_->GetMaxSize(), base::Log::config_->GetRotate());
    std::shared_ptr<spdlog::logger> instance_logger = spdlog::rotating_logger_mt("instance.log", base::Log::config_->GetInstanceLog(), 1048576 * base::Log::config_->GetMaxSize(), base::Log::config_->GetRotate());

    debug_logger->set_level(spdlog::level::debug);
    server_logger->set_level(spdlog::level::info);
    instance_logger->set_level(spdlog::level::info);

    debug_logger->set_pattern("%Y-%m-%d %H:%M:%S %z - %n - %l - %v");
    server_logger->set_pattern("%Y-%m-%d %H:%M:%S %z - %n - %l - %v");
    instance_logger->set_pattern("%Y-%m-%d %H:%M:%S %z - %n - %l - %v");

    debug_logger->flush_on(spdlog::level::debug);
    server_logger->flush_on(spdlog::level::info);
    instance_logger->flush_on(spdlog::level::info);

    this->inited = true;
    return true;
}

std::shared_ptr<spdlog::logger> base::Log::debug_logger()
{
    return spdlog::get("debug.log");
}

std::shared_ptr<spdlog::logger> base::Log::server_logger()
{
    return spdlog::get("server.log");
}

std::shared_ptr<spdlog::logger> base::Log::instance_logger()
{
    return spdlog::get("instance.log");
}