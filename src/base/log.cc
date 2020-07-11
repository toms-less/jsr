#include <include/base.h>

base::LogConfig *base::Log::config_ = nullptr;
spdlog::logger *base::Log::debug_logger_ = nullptr;
spdlog::logger *base::Log::server_logger_ = nullptr;
spdlog::logger *base::Log::instance_logger_ = nullptr;

base::Log::Log(base::LogConfig *config)
{
    base::Log::config_ = config;
}

base::Log::~Log()
{
}

bool base::Log::initialize()
{
    base::Log::debug_logger_ = spdlog::rotating_logger_mt("debug", base::Log::config_->GetDebugLog(), 1048576 * base::Log::config_->GetMaxSize(), base::Log::config_->GetRotate()).get();
    base::Log::server_logger_ = spdlog::rotating_logger_mt("server", base::Log::config_->GetServerLog(), 1048576 * base::Log::config_->GetMaxSize(), base::Log::config_->GetRotate()).get();
    base::Log::instance_logger_ = spdlog::rotating_logger_mt("instance", base::Log::config_->GetInstanceLog(), 1048576 * base::Log::config_->GetMaxSize(), base::Log::config_->GetRotate()).get();
    this->inited = true;
    return this->inited;
}

spdlog::logger *base::Log::debug_logger()
{
    return base::Log::debug_logger_;
}

spdlog::logger *base::Log::server_logger()
{
    return base::Log::server_logger_;
}

spdlog::logger *base::Log::instance_logger()
{
    return base::Log::instance_logger_;
}