#include <include/base.h>

base::Cookie::Cookie(const char *name, const char *value)
{
    name_.assign(name);
    value_.assign(value);
}

const std::string &base::Cookie::name()
{
    return name_;
}

const std::string &base::Cookie::value()
{
    return value_;
}

void base::Cookie::set_domain(const char *domain)
{
    domain_.assign(domain);
}

const std::string &base::Cookie::domain()
{
    return domain_;
}

void base::Cookie::set_path(const char *path)
{
    path_.assign(path);
}

const std::string &base::Cookie::path()
{
    return path_;
}

void base::Cookie::set_expires(const char *expires)
{
    expires_.assign(expires);
}

const std::string &base::Cookie::expires()
{
    return expires_;
}

void base::Cookie::set_max_age(int32_t max_age)
{
    max_age_ = max_age;
}

const int32_t &base::Cookie::max_age()
{
    return max_age_;
}

void base::Cookie::set_secure(bool secure)
{
    secure_ = secure;
}

const bool &base::Cookie::secure()
{
    return secure_;
}

void base::Cookie::set_http_only(bool http_only)
{
    http_only_ = http_only;
}

bool &base::Cookie::http_only()
{
    return http_only_;
}