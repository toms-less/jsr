#include <include/instance.h>

instance::InstanceConfig::InstanceConfig()
{
}

void instance::InstanceConfig::set_startup_data(const std::string &startup_data)
{
    startup_data_ = startup_data;
}

std::string &instance::InstanceConfig::startup_data()
{
    return startup_data_;
}

void instance::InstanceConfig::set_stack_limit(uint32_t limit)
{
    stack_limit_ = limit;
}

uint32_t instance::InstanceConfig::stack_limit()
{
    return stack_limit_;
}

void instance::InstanceConfig::set_physical_memory(uint64_t memory)
{
    physical_memory_ = memory;
}

uint64_t instance::InstanceConfig::physical_memory()
{
    return physical_memory_;
}

void instance::InstanceConfig::set_virtual_memory_limit(uint64_t limit)
{
    virtual_memory_limit_ = limit;
}

uint64_t instance::InstanceConfig::virtual_memory_limit()
{
    return virtual_memory_limit_;
}

void instance::InstanceConfig::set_max_old_generation_size(uint64_t size)
{
    max_old_generation_size_ = size;
}

uint64_t instance::InstanceConfig::max_old_generation_size()
{
    return max_old_generation_size_;
}

void instance::InstanceConfig::set_initial_old_generation_size(uint64_t size)
{
    initial_old_generation_size_ = size;
}

uint64_t instance::InstanceConfig::initial_old_generation_size()
{
    return initial_old_generation_size_;
}

void instance::InstanceConfig::set_max_young_generation_size(uint64_t size)
{
    max_young_generation_size_ = size;
}

uint64_t instance::InstanceConfig::max_young_generation_size()
{
    return max_young_generation_size_;
}

void instance::InstanceConfig::set_initial_young_generation_size(uint64_t size)
{
    initial_young_generation_size_ = size;
}

uint64_t instance::InstanceConfig::initial_young_generation_size()
{
    return initial_young_generation_size_;
}

void instance::InstanceConfig::set_instance_count(uint32_t count)
{
    instance_count_ = count;
}

uint32_t instance::InstanceConfig::instance_count()
{
    return instance_count_;
}