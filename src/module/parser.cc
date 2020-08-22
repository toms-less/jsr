#include <include/module.h>

void module::ScriptModule::deps_parse(module::DepsParseContext &context)
{
    std::string &specifier = context.specifier();
    if (specifier.empty())
    {
        context.set_error("Compile error, specifier in the module script is empty.");
        return;
    }

    if (specifier.find("//") == 0)
    {
        /**
         * ---lib1.js---
         * import {foo} from '//mod/test.js'
         * 
         */
        context.set_type(module::DepsType::LOCAL);
        local_deps_parse(context);
    }
    else if (specifier.find("http") == 0)
    {
        /**
         * ---lib2.js---
         * import {foo} from 'https://test.com/mod/test.js'
         * 
        */
        context.set_type(module::DepsType::HTTP);
        http_deps_parse(context);
    }
    else if (specifier.find("ftp") == 0)
    {
        /**
         * ---lib4.js---
         * import {foo} from 'ftp://test.com/mod/test.js' 
         * 
        */
        context.set_type(module::DepsType::FTP);
        ftp_deps_parse(context);
    }
    else if (specifier.find("@toms") == 0)
    {
        /**
         * ---lib4.js---
         * import {foo1,foo2} from '@toms//std/net.js'
         * 
        */
        context.set_type(module::DepsType::SYS);
        sys_deps_parse(context);
    }
    else if (specifier.find("@") == 0 && specifier.find("@toms") != 0)
    {
        /**
         * ---lib5.js---
         * import {foo1,foo2} from '@redux//lib/test.js' 
         * 
        */
        context.set_type(module::DepsType::THIRD_PARTY);
        third_deps_parse(context);
    }
    else
    {
        context.set_type(module::DepsType::UNKNOWN);
        context.set_error("Compile error, specifier in the module script is unsupported.");
    }
}

void module::ScriptModule::local_deps_parse(DepsParseContext &context)
{
}

void module::ScriptModule::http_deps_parse(DepsParseContext &context)
{
}

void module::ScriptModule::ftp_deps_parse(DepsParseContext &context)
{
}

void module::ScriptModule::sys_deps_parse(DepsParseContext &context)
{
}

void module::ScriptModule::third_deps_parse(DepsParseContext &context)
{
}