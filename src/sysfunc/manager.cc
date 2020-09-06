#include <include/sysfunc.h>

sysfunc::SystemFuncManager::SystemFuncManager(instance::IntanceManager &instance_manager) : instance_manager_(instance_manager)
{
}

bool sysfunc::SystemFuncManager::initialize()
{
    /**
     * build system function of 'console'.
     * 1. 'console.log';
     * 2. 'console.info';
     * 3. 'console.warn';
     * 4. 'console.debug';
     * 5. 'console.error';
     * 6. 'console.time';
     * 7. 'console.timeEnd';
     * 
    */
    map_.insert(std::pair<std::string, pfunc>("sysfunc::Console::log", sysfunc::Console::log));
    map_.insert(std::pair<std::string, pfunc>("sysfunc::Console::info", sysfunc::Console::info));
    map_.insert(std::pair<std::string, pfunc>("sysfunc::Console::warn", sysfunc::Console::warn));
    map_.insert(std::pair<std::string, pfunc>("sysfunc::Console::debug", sysfunc::Console::debug));
    map_.insert(std::pair<std::string, pfunc>("sysfunc::Console::error", sysfunc::Console::error));
    map_.insert(std::pair<std::string, pfunc>("sysfunc::Console::time", sysfunc::Console::time));
    map_.insert(std::pair<std::string, pfunc>("sysfunc::Console::timeEnd", sysfunc::Console::timeEnd));

    /**
     * build system function of 'http'.
     * 1. 'http.get';
     * 2. 'http.post';
     * 3. 'http.options()';
     * 4. 'http.patch()';
     * 5. 'http.put()';
     * 6. 'http.delete()';
     * 
    */
    map_.insert(std::pair<std::string, pfunc>("sysfunc::Http::get", sysfunc::Http::get));
    map_.insert(std::pair<std::string, pfunc>("sysfunc::Http::post", sysfunc::Http::post));
    map_.insert(std::pair<std::string, pfunc>("sysfunc::Http::options", sysfunc::Http::options));
    map_.insert(std::pair<std::string, pfunc>("sysfunc::Http::patch", sysfunc::Http::patch));
    map_.insert(std::pair<std::string, pfunc>("sysfunc::Http::put", sysfunc::Http::put));
    map_.insert(std::pair<std::string, pfunc>("sysfunc::Http::del", sysfunc::Http::del));

    auto instance_log = base::Log::instance_logger();
    for (instance::Instance *instance : instance_manager_.instances())
    {
        instance::BindObjectContext deps_ctx("sysfunc", "deps", sysfunc::SysFunc::deps);
        instance->bind_object(deps_ctx);
        if (!deps_ctx.ok())
        {
            instance_log->error(deps_ctx.error());
            continue;
        }

        instance::BindObjectContext bind_ctx("sysfunc", "bind", sysfunc::SysFunc::bind);
        instance->bind_object(deps_ctx);
        if (!bind_ctx.ok())
        {
            instance_log->error(bind_ctx.error());
            continue;
        }
    }
    return true;
}