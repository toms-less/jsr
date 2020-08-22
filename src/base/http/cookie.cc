#include <include/base.h>

base::http::Cookie::Cookie()
{
}

base::http::Cookie::Cookie(const char *name, const char *value)
{
    name_.assign(name);
    value_.assign(value);
}

void base::http::Cookie::set_name(const char *name)
{
    name_.assign(name);
}

const std::string &base::http::Cookie::name()
{
    return name_;
}

void base::http::Cookie::set_value(const char *value)
{
    value_.assign(value);
}

const std::string &base::http::Cookie::value()
{
    return value_;
}

void base::http::Cookie::set_domain(const char *domain)
{
    domain_.assign(domain);
}

const std::string &base::http::Cookie::domain()
{
    return domain_;
}

void base::http::Cookie::set_path(const char *path)
{
    path_.assign(path);
}

const std::string &base::http::Cookie::path()
{
    return path_;
}

void base::http::Cookie::set_expires(const char *expires)
{
    expires_.assign(expires);
}

const std::string &base::http::Cookie::expires()
{
    return expires_;
}

void base::http::Cookie::set_max_age(int32_t max_age)
{
    max_age_ = max_age;
}

const int32_t &base::http::Cookie::max_age()
{
    return max_age_;
}

void base::http::Cookie::set_secure(bool secure)
{
    secure_ = secure;
}

const bool &base::http::Cookie::secure()
{
    return secure_;
}

void base::http::Cookie::set_http_only(bool http_only)
{
    http_only_ = http_only;
}

bool &base::http::Cookie::http_only()
{
    return http_only_;
}