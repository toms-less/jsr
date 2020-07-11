#include <include/instance.h>

instance::InstanceConfig::InstanceConfig()
{
}

void instance::InstanceConfig::SetStartupData(const std::string &_startupData)
{
    this->startupData = _startupData;
}

std::string &instance::InstanceConfig::GetStartupData()
{
    return this->startupData;
}

void instance::InstanceConfig::SetStackLimit(uint32_t _stackLimit)
{
    this->stackLimit = _stackLimit;
}

uint32_t instance::InstanceConfig::GetStackLimit()
{
    return this->stackLimit;
}

void instance::InstanceConfig::SetPhysicalMemory(uint64_t _physicalMemory)
{
    this->physicalMemory = _physicalMemory;
}

uint64_t instance::InstanceConfig::GetPhysicalMemory()
{
    return this->physicalMemory;
}

void instance::InstanceConfig::SetVirtualMemoryLimit(uint64_t _virtualMemoryLimit)
{
    this->virtualMemoryLimit = _virtualMemoryLimit;
}

uint64_t instance::InstanceConfig::GetVirtualMemoryLimit()
{
    return this->virtualMemoryLimit;
}

void instance::InstanceConfig::SetMaxOldeGenerationSize(uint64_t _maxOldeGenerationSize)
{
    this->maxOldeGenerationSize = _maxOldeGenerationSize;
}

uint64_t instance::InstanceConfig::GetMaxOldeGenerationSize()
{
    return this->maxOldeGenerationSize;
}

void instance::InstanceConfig::SetInitialOldGenerationSize(uint64_t _initialOldGenerationSize)
{
    this->initialOldGenerationSize = _initialOldGenerationSize;
}

uint64_t instance::InstanceConfig::GetInitialOldGenerationSize()
{
    return this->initialOldGenerationSize;
}

void instance::InstanceConfig::SetMaxYounGenerationSize(uint64_t _maxYounGenerationSize)
{
    this->maxYounGenerationSize = _maxYounGenerationSize;
}

uint64_t instance::InstanceConfig::GetMaxYounGenerationSize()
{
    return this->maxYounGenerationSize;
}

void instance::InstanceConfig::SetInitialYoungGenerationSize(uint64_t _initialYoungGenerationSize)
{
    this->initialYoungGenerationSize = _initialYoungGenerationSize;
}

uint64_t instance::InstanceConfig::GetInitialYoungGenerationSize()
{
    return this->initialYoungGenerationSize;
}

void instance::InstanceConfig::SetCodeRangeSize(uint64_t _codeRangeSize)
{
    this->codeRangeSize = _codeRangeSize;
}

uint64_t instance::InstanceConfig::GetCodeRangeSize()
{
    return this->codeRangeSize;
}

void instance::InstanceConfig::SetMaxZonePoolSize(uint64_t _maxZonePoolSize)
{
    this->maxZonePoolSize = _maxZonePoolSize;
}

uint64_t instance::InstanceConfig::GetMaxZonePoolSize()
{
    return this->maxZonePoolSize;
}

void instance::InstanceConfig::SetInstanceCount(uint32_t _instanceCount)
{
    this->instanceCount = _instanceCount;
}

uint32_t instance::InstanceConfig::GetInstanceCount()
{
    return this->instanceCount;
}