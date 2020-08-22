#include <include/module.h>

module::DepsParseContext::DepsParseContext(const char *specifier)
{
    specifier_.assign(specifier);
    ok_ = false;
}

std::string &module::DepsParseContext::specifier()
{
    return specifier_;
}

void module::DepsParseContext::set_type(DepsType type)
{
    type_ = type;
}

module::DepsType &module::DepsParseContext::type()
{
    return type_;
}

void module::DepsParseContext::set_script(const char *script)
{
    script_.assign(script);
}

std::string &module::DepsParseContext::script()
{
    return script_;
}

void module::DepsParseContext::set_ok()
{
    ok_ = true;
}

bool &module::DepsParseContext::ok()
{
    return ok_;
}

void module::DepsParseContext::set_error(const char *error)
{
    error_.assign(error);
}

std::string &module::DepsParseContext::error()
{
    return error_;
}
