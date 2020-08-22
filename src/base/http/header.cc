#include <include/base.h>

base::http::Header::Header(const char *name, const char *value)
{
    name_.assign(name);
    value_.assign(value);
}

const std::string &base::http::Header::name()
{
    return name_;
}

const std::string &base::http::Header::value()
{
    return value_;
}