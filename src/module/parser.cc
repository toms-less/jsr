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
    if (context.repository().empty())
    {
        context.set_error("Compile error, repository is empty.");
        return;
    }
    if (context.repo_ns().empty())
    {
        context.set_error("Compile error, repository namespace is empty.");
        return;
    }
    if (context.specifier().length() <= 2)
    {
        context.set_error("Compile error, specifier is invalid.");
        return;
    }
    if (context.project().empty())
    {
        context.set_error("Compile error, project name is empty in the 'workspace.jsr'.");
        return;
    }
    std::string end = "/";
    std::string url = context.repository();
    if (end.size() < context.repository().size() && context.repository().rfind(end) == (context.repository().length() - end.length()))
    {
        // Repository is end of '/'.
        url.append(context.project()).append("/").append(context.specifier().substr(2));
    }
    else
    {
        url.append("/").append(context.project()).append("/").append(context.specifier().substr(2));
    }

    base::http::HttpEntry entry(url.c_str());
    entry.set_request_header("namespace", context.repo_ns().c_str());
    base::http::HttpClient client;
    client.sync_get(entry);
    if (!entry.ok())
    {
        context.set_error(entry.error().c_str());
        return;
    }
    if (entry.status() != 200)
    {
        context.set_error(entry.status_message().c_str());
        return;
    }

    const std::string &script_content = entry.response_content();
    if (script_content.empty())
    {
        std::string error = "Compile error, current dependency script from current specifier '" + context.specifier() + "' is empty.";
        context.set_error(error.c_str());
        return;
    }
    context.set_script(script_content.c_str());
    context.set_ok();
}

void module::ScriptModule::http_deps_parse(DepsParseContext &context)
{
    /**
     * To make sure the dependency script is fixed,
     * it can not be has the parameter options.
     * 
    */
    if (context.specifier().find("?") != std::string::npos)
    {
        std::string error = "Compile error, current specifier '" + context.specifier() + "' has parameter character '?'.";
        context.set_error(error.c_str());
        return;
    }

    base::http::HttpEntry entry(context.specifier().c_str());
    base::http::HttpClient client;
    client.sync_get(entry);
    if (!entry.ok())
    {
        context.set_error(entry.error().c_str());
        return;
    }
    if (entry.status() != 200)
    {
        context.set_error(entry.status_message().c_str());
        return;
    }

    const std::string &script_content = entry.response_content();
    if (script_content.empty())
    {
        std::string error = "Compile error, current dependency script from current specifier '" + context.specifier() + "' is empty.";
        context.set_error(error.c_str());
        return;
    }
    context.set_script(script_content.c_str());
    context.set_ok();
}

void module::ScriptModule::ftp_deps_parse(DepsParseContext &context)
{
    /**
     * TODO:
     * support it.
     * 
    */
    context.set_error("Compile error, FTP dependency is unsupported now.");
}

void module::ScriptModule::sys_deps_parse(DepsParseContext &context)
{
    /**
     * TODO:
     * support it.
     * 
    */
    context.set_error("Compile error, system dependency is unsupported now.");
}

void module::ScriptModule::third_deps_parse(DepsParseContext &context)
{
    /**
     * TODO:
     * support it.
     * 
    */
    context.set_error("Compile error, third dependency is unsupported now.");
}