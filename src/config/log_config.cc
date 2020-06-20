#include <include/base.h>

base::LogConfig::LogConfig()
{
}

base::LogConfig::~LogConfig()
{
}

void base::LogConfig::SetDebugLog(const std::string &_debugLog)
{
    this->debugLog = _debugLog;
}

std::string &base::LogConfig::GetDebugLog()
{
    return this->debugLog;
}

void base::LogConfig::SetServerLog(const std::string &_serverLog)
{
    this->serverLog = _serverLog;
}

std::string &base::LogConfig::GetServerLog()
{
    return this->serverLog;
}

void base::LogConfig::SetInstanceLog(const std::string &_instanceLog)
{
    this->instanceLog = _instanceLog;
}

std::string &base::LogConfig::GetInstanceLog()
{
    return this->instanceLog;
}

void base::LogConfig::SetMaxSize(int _size)
{
    this->maxSize = _size;
}

int base::LogConfig::GetMaxSize()
{
    return this->maxSize;
}

void base::LogConfig::SetRotate(int _rotate)
{
    this->rotate = _rotate;
}

int base::LogConfig::GetRotate()
{
    return this->rotate;
}