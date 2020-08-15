#include <include/base.h>

base::Header::Header(const char *name, const char *value)
{
    name_.assign(name);
    value_.assign(value);
}

const std::string &base::Header::name()
{
    return name_;
}

const std::string &base::Header::value()
{
    return value_;
}