#include <include/base.h>

base::LogConfig *base::Log::logConfig = nullptr;

base::Log::Log(base::LogConfig *_logConfig)
{
    base::Log::logConfig = _logConfig;
}

base::Log::~Log()
{
}

bool base::Log::initialize()
{
    this->inited = true;
    return this->inited;
}

std::shared_ptr<spdlog::logger> base::Log::GetDebugLogger()
{
    return spdlog::rotating_logger_mt("debug", base::Log::logConfig->GetDebugLog(), 1048576 * base::Log::logConfig->GetMaxSize(), base::Log::logConfig->GetRotate());
}

std::shared_ptr<spdlog::logger> base::Log::GetServerLogger()
{
    return spdlog::rotating_logger_mt("server", base::Log::logConfig->GetServerLog(), 1048576 * base::Log::logConfig->GetMaxSize(), base::Log::logConfig->GetRotate());
}

std::shared_ptr<spdlog::logger> base::Log::GetInstanceLogger()
{
    return spdlog::rotating_logger_mt("instance", base::Log::logConfig->GetInstanceLog(), 1048576 * base::Log::logConfig->GetMaxSize(), base::Log::logConfig->GetRotate());
}