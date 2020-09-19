#include <include/base.h>

void base::LogConfig::set_debug_log(const std::string &file)
{
    debug_log_file_ = file;
}

std::string &base::LogConfig::debug_log()
{
    return debug_log_file_;
}

void base::LogConfig::set_server_log(const std::string &file)
{
    server_log_file_ = file;
}

std::string &base::LogConfig::server_log()
{
    return server_log_file_;
}

void base::LogConfig::set_instance_log(const std::string &file)
{
    instance_log_file_ = file;
}

std::string &base::LogConfig::instance_log()
{
    return instance_log_file_;
}

void base::LogConfig::set_max_size(int size)
{
    max_size_ = size;
}

int base::LogConfig::max_size()
{
    return max_size_;
}

void base::LogConfig::set_rotate(int rotate)
{
    rotate_ = rotate;
}

int base::LogConfig::rotate()
{
    return rotate_;
}