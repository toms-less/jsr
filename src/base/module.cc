#include <include/base.h>

base::Module::Module()
{
}

base::Module::~Module()
{
}

void base::Module::SetInited(bool _inited)
{
    this->inited = _inited;
}

bool base::Module::IsInited()
{
    return this->inited;
}