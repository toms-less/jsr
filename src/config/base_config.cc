#include <include/base.h>

base::BaseConfig::BaseConfig()
{
}

base::BaseConfig::~BaseConfig()
{
}

void base::BaseConfig::SetInited(bool _inited)
{
    this->inited = _inited;
}

bool base::BaseConfig::IsInited()
{
    return this->inited;
}